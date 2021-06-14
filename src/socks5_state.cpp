#include "../include/socks5_state.h"
#include "../include/socks5.h"
#include "../include/socks5_session.h"
#include "../include/server_stream.h"
#include "../include/client_stream.h"
#include "../logger/logger.h"
#include <boost/format.hpp>

using fmt = boost::format;
using logger = logging::logger;

void socks5_state::handle(socks5_session *session, io_event& event, server_stream_ptr& stream) {}
void socks5_state::handle(socks5_session *session, io_event& event, client_stream_ptr& stream) {}

void socks5_state::change_state(socks5_session *session, std::unique_ptr<socks5_state> state) {
    session->change_state(move(state));
}

void socks5_auth_request::handle(socks5_session *session, io_event &event, server_stream_ptr& stream) {
    auto ctx = session->context();
    auto is_socks5_auth_request = socks5::is_auth_request(event.buffer.data(), event.buffer.size());

    ctx.response.resize(2);
    ctx.response[0] = socks5::proto::version;
    ctx.response[1] = is_socks5_auth_request ? proto::auth::kNoAuth : proto::auth::kNotSupported;
    if (ctx.response[1] == proto::auth::kNotSupported)
        logger::warning((fmt("[%1%] socks5 protocol: auth request is not supported") % stream->id()).str());

    io_event new_event{event.id, io_event::none, ctx.response.data(), ctx.response.size()};
    ctx.server->write(std::move(new_event));

    change_state(session, socks5_connection_request::instance());
}

void socks5_auth_request::handle(socks5_session *session, io_event &event, client_stream_ptr& stream) {}

void socks5_connection_request::handle(socks5_session *session, io_event &event, server_stream_ptr& stream) {
    auto &ctx = session->context();

    if (event.type == io_event::write) {
        ctx.server->read();
        return;
    }

    std::string host, service;
    if (!socks5::is_valid_request_packet(event.buffer.data(), event.buffer.size())) {
        logger::error((fmt("[%1%] socks5 protocol: bad request packet") % stream->id()).str());
        stream->stop();
        return;
    }

    if (!socks5::get_remote_address_info(event.buffer.data(), event.buffer.size(), host, service)) {
        logger::error((fmt("[%1%] socks5 protocol: bad remote address format") % stream->id()).str());
        stream->stop();
        return;
    }

    logger::info((fmt("[%1%] requested [%2%:%3%]") % stream->id() % host % service).str());
    ctx.response.resize(event.buffer.size());
    copy(event.buffer.begin(), event.buffer.end(), ctx.response.begin());
    ctx.client->set_host(host);
    ctx.client->set_service(service);
    ctx.client->start();
    session->change_state(socks5_connection_established::instance());
}

void socks5_connection_request::handle(socks5_session *session, io_event &event, client_stream_ptr& stream) {}

void socks5_connection_established::handle(socks5_session *session, io_event &event, server_stream_ptr& stream) {}

void socks5_connection_established::handle(socks5_session *session, io_event &event, client_stream_ptr& stream) {
    if (event.type == io_event::connect) {
        auto& ctx = session->context();
        (reinterpret_cast<socks5::request_header *>(ctx.response.data()))->command = 0x00;
        io_event new_event{ctx.id, io_event::none, ctx.response.data(), ctx.response.size()};
        ctx.server->write(std::move(new_event));
        session->change_state(socks5_ready_to_transfer_data::instance());
    }
}

void socks5_ready_to_transfer_data::handle(socks5_session *session, io_event &event, server_stream_ptr& stream) {
    session->context().server->read();
    session->context().client->read();
    session->change_state(socks5_data_transfer_mode::instance());
}

void socks5_ready_to_transfer_data::handle(socks5_session *session, io_event &event, client_stream_ptr& stream) {}

void socks5_data_transfer_mode::handle(socks5_session *session, io_event &event, server_stream_ptr& stream) {
    auto& ctx{session->context()};

    if (event.type == io_event::write) {
        ctx.client->read();
    } else if (event.type == io_event::read) {
        ctx.transferred_bytes_to_remote += event.buffer.size();
        ctx.client->write(std::move(event));
    } else {
        logger::error((fmt("[%1%] transfer mode logic error") % stream->id()).str());
    }
}

void socks5_data_transfer_mode::handle(socks5_session *session, io_event &event, client_stream_ptr& stream) {
    auto& ctx{session->context()};

    if (event.type == io_event::write) {
        ctx.server->read();
    } else if (event.type == io_event::read) {
        ctx.transferred_bytes_to_local += event.buffer.size();
        ctx.server->write(std::move(event));
    } else {
        logger::error((fmt("[%1%] transfer mode logic error") % stream->id()).str());
    }
}
