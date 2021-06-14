#ifndef SOCKS5_PROXY_SOCKS5_STATE_H
#define SOCKS5_PROXY_SOCKS5_STATE_H

#include <memory>
#include "io_event.h"

class server_stream;
class client_stream;
using server_stream_ptr = std::shared_ptr<server_stream>;
using client_stream_ptr = std::shared_ptr<client_stream>;

class socks5_session;

class socks5_state {
public:
    virtual void handle(socks5_session *session, io_event& event, server_stream_ptr& stream);
    virtual void handle(socks5_session *session, io_event& event, client_stream_ptr& stream);
protected:
    static void change_state(socks5_session* session, std::unique_ptr<socks5_state> state);
};

class socks5_auth_request final : public socks5_state {
public:
    static auto instance() { return std::make_unique<socks5_auth_request>(); }
    void handle(socks5_session *session, io_event& event, server_stream_ptr& stream) override;
    void handle(socks5_session *session, io_event& event, client_stream_ptr& stream) override;
};

class socks5_connection_request final : public socks5_state {
public:
    static auto instance() { return std::make_unique<socks5_connection_request>(); }
    void handle(socks5_session *session, io_event& event, server_stream_ptr& stream) override;
    void handle(socks5_session *session, io_event& event, client_stream_ptr& stream) override;
};

class socks5_connection_established final : public socks5_state {
public:
    static auto instance() { return std::make_unique<socks5_connection_established>(); }
    void handle(socks5_session *session, io_event& event, server_stream_ptr& stream) override;
    void handle(socks5_session *session, io_event& event, client_stream_ptr& stream) override;
};

class socks5_ready_to_transfer_data final : public socks5_state {
public:
    static auto instance() { return std::make_unique<socks5_ready_to_transfer_data>(); }
    void handle(socks5_session *session, io_event& event, server_stream_ptr& stream) override;
    void handle(socks5_session *session, io_event& event, client_stream_ptr& stream) override;
};

class socks5_data_transfer_mode final : public socks5_state {
public:
    static auto instance() { return std::make_unique<socks5_data_transfer_mode>(); }
    void handle(socks5_session *session, io_event& event, server_stream_ptr& stream) override;
    void handle(socks5_session *session, io_event& event, client_stream_ptr& stream) override;
};

#endif //SOCKS5_PROXY_SOCKS5_STATE_H
