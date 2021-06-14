#include "tcp_server_stream.h"
#include "stream_manager.h"
#include <boost/format.hpp>
#include "../logger/logger.h"

using fmt = boost::format;
using logger = logging::logger;

tcp_server_stream::tcp_server_stream(const stream_manager_ptr& ptr, int id, const net::executor& ctx)
    : server_stream(ptr, id), socket_(ctx), read_buffer_{}, write_buffer_{} {
}

tcp_server_stream::~tcp_server_stream() {
    auto str = (fmt("[%1%] tcp server stream closed") % id()).str();
    logger::trace(str);
}

net::executor tcp_server_stream::executor() { return socket_.get_executor(); }

tcp::socket& tcp_server_stream::socket() { return socket_; }

void tcp_server_stream::do_start() {
    auto str{(fmt("[%1%] incoming connection from socks5-client: [%2%]")
              % id() % remote_ep_str()).str()};
    logger::debug(str);
    do_read();
}

void tcp_server_stream::do_stop() {
    sys::error_code ignored_ec;
    socket_.shutdown(tcp::socket::shutdown_both, ignored_ec);
}

void tcp_server_stream::do_write(io_event event) {
    copy(event.buffer.begin(), event.buffer.end(), write_buffer_.begin());
    net::async_write(
            socket_, net::buffer(write_buffer_, event.buffer.size()),
            [this, self{shared_from_this()}](const sys::error_code &ec, size_t) {
                if (!ec) {
                    io_event event{id(), io_event::write};
                    manager()->notify(std::move(event), shared_from_this());
                } else {
                    close(ec);
                }
            });
}

void tcp_server_stream::do_read() {
    socket_.async_read_some(
            net::buffer(read_buffer_),
            [this, self{shared_from_this()}](const sys::error_code &ec, const size_t length) {
                if (!ec) {
                    io_event event(id(), io_event::read, read_buffer_.data(), length);
                    manager()->notify(std::move(event), shared_from_this());
                } else {
                    close(ec);
                }
            });
}

void tcp_server_stream::close(const sys::error_code& ec) {
    io_event event{id(), io_event::close};

    if (ec && ec.value() != net::error::eof) {
        const std::string error{move(ec.message())};
        event.buffer.assign(error.begin(), error.end());
        event.type = io_event::error;
    }

    manager()->notify(std::move(event), shared_from_this());
}

std::string tcp_server_stream::ep_to_str(const tcp::endpoint& ep) {
    std::string ep_str{ep.address().to_string() + ":" + std::to_string(ep.port())};
    return ep_str;
}

std::string tcp_server_stream::remote_ep_str() const {
    return ep_to_str(socket_.remote_endpoint());
}

std::string tcp_server_stream::local_ep_str() const {
    return ep_to_str(socket_.local_endpoint());
}






