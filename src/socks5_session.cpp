#include "../include/socks5_session.h"
#include "../include/server_stream.h"
#include "../include/client_stream.h"

socks5_session::socks5_session(int id, server_stream_ptr server_stream, client_stream_ptr client_stream)
    : context_{id, std::move(server_stream), std::move(client_stream), {}, 0, 0} {
    state_ = socks5_auth_request::instance();
}

void socks5_session::change_state(std::unique_ptr<socks5_state> state) {
    state_ = move(state);
}

void socks5_session::handle(io_event &event, server_stream_ptr stream) {
    state_->handle(this, event, stream);
}

void socks5_session::handle(io_event &event, client_stream_ptr stream) {
    state_->handle(this, event, stream);
}

void socks5_session::stop() {
    if (context_.server)
        context_.server->stop();
    if (context_.client)
        context_.client->stop();
}
