#include "../include/socks5_state.h"
#include "../include/socks5.h"
#include "../include/socks5_session.h"
#include "../logger/logger.h"
#include "../include/stream_manager.h"
#include <boost/format.hpp>

using fmt = boost::format;
using logger = logging::logger;

void socks5_state::handle_server_read(socks5_session *session, io_event& event) {}
void socks5_state::handle_client_read(socks5_session *session, io_event& event) {}
void socks5_state::handle_client_connect(socks5_session *session, io_event& event) {}
void socks5_state::handle_server_write(socks5_session *session, io_event& event) {}
void socks5_state::handle_client_write(socks5_session *session, io_event& event) {}
void socks5_state::change_state(socks5_session *session, std::unique_ptr<socks5_state> state) {
    session->change_state(move(state));
}

void socks5_auth_request::handle_server_read(socks5_session *session, io_event &event) {
    auto ctx = session->context();
    auto is_socks5_auth_request = socks5::is_auth_request(event.buffer.data(), event.buffer.size());

    ctx.response.resize(2);
    ctx.response[0] = socks5::proto::version;
    ctx.response[1] = is_socks5_auth_request ? proto::auth::kNoAuth : proto::auth::kNotSupported;
    if (ctx.response[1] == proto::auth::kNotSupported)
        logger::warning((fmt("[%1%] socks5 protocol: auth request is not supported") % ctx.id).str());

    io_event new_event{event.id, io_event::none, ctx.response};
    session->manager()->write_server(ctx.id, std::move(new_event));

    change_state(session, socks5_connection_request::instance());
}

void socks5_connection_request::handle_server_write(socks5_session *session, io_event &event) {
    session->manager()->read_server(session->context().id);
}

void socks5_connection_request::handle_server_read(socks5_session *session, io_event &event) {
    auto &ctx = session->context();

    std::string host, service;
    if (!socks5::is_valid_request_packet(event.buffer.data(), event.buffer.size())) {
        logger::error((fmt("[%1%] socks5 protocol: bad request packet") % ctx.id).str());
        session->manager()->stop(ctx.id);
        return;
    }

    if (!socks5::get_remote_address_info(event.buffer.data(), event.buffer.size(), host, service)) {
        logger::error((fmt("[%1%] socks5 protocol: bad remote address format") % ctx.id).str());
        session->manager()->stop(ctx.id);
        return;
    }

    logger::info((fmt("[%1%] requested [%2%:%3%]") % ctx.id % host % service).str());
    ctx.response.resize(event.buffer.size());
    std::ranges::copy(event.buffer, ctx.response.begin());
    session->manager()->connect(ctx.id, std::move(host), std::move(service));
    session->change_state(socks5_connection_established::instance());
}

void socks5_connection_established::handle_client_connect(socks5_session *session, io_event &event) {
    auto& ctx = session->context();
    (reinterpret_cast<socks5::request_header*>(ctx.response.data()))->command = 0x00;
    io_event new_event{ctx.id, io_event::none, ctx.response};
    session->manager()->write_server(ctx.id, std::move(new_event));
    session->change_state(socks5_ready_to_transfer_data::instance());
}

void socks5_ready_to_transfer_data::handle_server_write(socks5_session *session, io_event &event) {
    auto& ctx{session->context()};
    session->manager()->read_server(ctx.id);
    session->manager()->read_client(ctx.id);
    session->change_state(socks5_data_transfer_mode::instance());
}

void socks5_data_transfer_mode::handle_server_write(socks5_session *session, io_event &event) {
    auto& ctx{session->context()};
    session->manager()->read_client(ctx.id);
}

void socks5_data_transfer_mode::handle_server_read(socks5_session *session, io_event &event) {
    auto& ctx{session->context()};
    ctx.transferred_bytes_to_remote += event.buffer.size();
    session->manager()->write_client(ctx.id, std::move(event));
}

void socks5_data_transfer_mode::handle_client_write(socks5_session *session, io_event &event) {
    auto& ctx{session->context()};
    session->manager()->read_server(ctx.id);
}

void socks5_data_transfer_mode::handle_client_read(socks5_session *session, io_event &event) {
    auto& ctx{session->context()};
    ctx.transferred_bytes_to_local += event.buffer.size();
    session->manager()->write_server(ctx.id, std::move(event));
}
