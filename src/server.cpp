#include <charconv>
#include <memory>
#include "../include/server.h"
#include "../include/tcp_server_stream.h"
#include "../logger/logger.h"

server::server(const std::string &port)
    : signals_(ctx_), acceptor_(ctx_), stream_id_(0) {
    configure_signals();
    async_wait_signals();

    stream_manager_ = std::make_shared<socks5_stream_manager>();

    uint16_t listen_port{0};
    std::from_chars(port.data(), port.data() + port.size(), listen_port);

    tcp::endpoint ep{tcp::endpoint(tcp::v4(), listen_port)};
    acceptor_.open(ep.protocol());
    acceptor_.set_option(tcp::acceptor::reuse_address(true));
    acceptor_.bind(ep);
    acceptor_.listen();

    logging::logger::info("socks5-proxy server starts on port: " + port);
    start_accept();
}

void server::run() {
    ctx_.run();
}

void server::configure_signals() {
    signals_.add(SIGINT);
    signals_.add(SIGTERM);
    signals_.add(SIGQUIT);
}

void server::async_wait_signals() {
    signals_.async_wait(
        [this](sys::error_code /*ec*/, int /*signno*/) {
            logging::logger::info("socks5-proxy server stopping");
            acceptor_.close();
            ctx_.stop();
            logging::logger::info("socks5-proxy server stopped");
        });
}

void server::start_accept() {
    auto ex{acceptor_.get_executor()};
    auto new_stream = std::make_shared<tcp_server_stream>(stream_manager_, ++stream_id_, ex);
    acceptor_.async_accept(
        new_stream->socket(),
        [this, new_stream](const sys::error_code &ec) {
            if (!acceptor_.is_open())
                return;

            if (!ec)
                stream_manager_->start(new_stream);

            start_accept();
        });
}

server::~server() {
    logging::logger::trace("socks5-proxy server stopped");
}

