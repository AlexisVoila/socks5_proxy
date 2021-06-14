#ifndef SOCKS5_PROXY_SOCKS5_SESSION_H
#define SOCKS5_PROXY_SOCKS5_SESSION_H

#include "socks5_state.h"

class server_stream;
class client_stream;
using client_stream_ptr = std::shared_ptr<client_stream>;
using server_stream_ptr = std::shared_ptr<server_stream>;

struct socks_ctx {
    int id;
    server_stream_ptr server;
    client_stream_ptr client;
    std::vector<uint8_t> response;
    std::size_t transferred_bytes_to_remote;
    std::size_t transferred_bytes_to_local;
};

class socks5_session {
public:
    socks5_session(int id, server_stream_ptr server_stream, client_stream_ptr client_stream);
    void change_state(std::unique_ptr<socks5_state> state);
    void handle(io_event& event, server_stream_ptr stream);
    void handle(io_event& event, client_stream_ptr stream);

    auto& context() { return context_; }
    void stop();

private:
    socks_ctx context_;
    std::unique_ptr<socks5_state> state_;
};

#endif //SOCKS5_PROXY_SOCKS5_SESSION_H
