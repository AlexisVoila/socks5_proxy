#include "tcp_client_stream.h"
#include "stream_manager.h"
#include "../logger/logger.h"
#include <boost/format.hpp>

using fmt = boost::format;
using logger = logging::logger;

tcp_client_stream::tcp_client_stream(const stream_manager_ptr& ptr, int id, const net::executor& ctx)
    : client_stream(ptr, id)
    , socket_(ctx)
    , resolver_(ctx), read_buffer_{}, write_buffer_{} {
}

tcp_client_stream::~tcp_client_stream() {
    auto str = (fmt("[%1%] tcp client stream closed (%2%:%3%)") % id() % host_ % port_).str();
    logger::trace(str);
}

void tcp_client_stream::do_start() {
    resolver_.async_resolve(
        host_, port_,
        [this, self{shared_from_this()}] (const sys::error_code& ec, const tcp::resolver::results_type& ep_iter) {
            if (!ec) {
                logger::info((fmt("[%1%] resolved [%2%] --> [%3%]\n")
                              % id() % host_ % ep_iter->endpoint().address().to_string()).str());
                do_connect(ep_iter);
            } else {
                close(ec);
            }
        });
}

void tcp_client_stream::do_stop() {
    sys::error_code ignored_ec;
    socket_.shutdown(tcp::socket::shutdown_both, ignored_ec);
}

void tcp_client_stream::do_connect(const tcp::resolver::results_type& ep_iter) {
    net::async_connect(
        socket_, ep_iter,
        [this, self{shared_from_this()}](const sys::error_code& ec, const tcp::endpoint& ep) {
            if (!ec) {
                logger::info((fmt("[%1%] connected to remote address [%2%]") % id() % remote_ep_str()));
                logger::debug((fmt("[%1%] local address [%2%]") % id() % local_ep_str()));
                io_event event(id(), io_event::connect);
                manager()->notify(std::move(event), shared_from_this());
            } else {
                close(ec);
            }
        });
}

void tcp_client_stream::do_write(io_event event) {
    copy(event.buffer.begin(), event.buffer.end(), write_buffer_.begin());
    net::async_write(
        socket_, net::buffer(write_buffer_, event.buffer.size()),
        [this, self{shared_from_this()}] (const sys::error_code& ec, std::size_t) {
            if (!ec) {
                io_event event{id(), io_event::write};
                manager()->notify(std::move(event), shared_from_this());
            } else {
                close(ec);
            }
        });
}

void tcp_client_stream::do_read() {
    socket_.async_read_some(
        net::buffer(read_buffer_),
        [this, self{shared_from_this()}] (const sys::error_code& ec, const std::size_t length) {
            if (!ec) {
                if (length) {
                    io_event event{id(), io_event::read, read_buffer_.data(), length};
                    manager()->notify(std::move(event), shared_from_this());
                } else {
                    close(ec);
                }
            } else {
                close(ec);
            }
        });
}

void tcp_client_stream::close(const sys::error_code& ec) {
    io_event event{id(), io_event::close};

    if (ec && ec.value() != net::error::eof) {
        const std::string error{std::move(ec.message())};
        event.buffer.assign(error.begin(), error.end());
        event.type = io_event::error;
    }

    manager()->notify(std::move(event), shared_from_this());
}

void tcp_client_stream::do_set_host(const std::string& host) { host_ = host; }

void tcp_client_stream::do_set_service(const std::string& service) { port_ = service; }

std::string tcp_client_stream::ep_to_str(const tcp::endpoint& ep) {
    std::string ep_str{ep.address().to_string() + ":" + std::to_string(ep.port())};
    return ep_str;
}

std::string tcp_client_stream::remote_ep_str() const {
    return ep_to_str(socket_.remote_endpoint());
}

std::string tcp_client_stream::local_ep_str() const {
    return ep_to_str(socket_.local_endpoint());
}
