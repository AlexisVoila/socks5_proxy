#ifndef SOCKS5_PROXY_SOCKS5_STREAM_MANAGER_H
#define SOCKS5_PROXY_SOCKS5_STREAM_MANAGER_H

#include "stream_manager.h"
#include "socks5_session.h"


class socks5_stream_manager final
    : public stream_manager
    , public std::enable_shared_from_this<socks5_stream_manager> {
public:
    socks5_stream_manager() = default;
    ~socks5_stream_manager() = default;

    socks5_stream_manager(const socks5_stream_manager& other) = delete;
    socks5_stream_manager& operator=(const socks5_stream_manager& other) = delete;

    void stop(stream_ptr stream) override;
    void stop(int id) override;
    void on_close(io_event event, stream_ptr stream) override;
    void on_error(io_event event, stream_ptr stream) override;

    void on_accept(server_stream_ptr stream) override;
    void on_read(io_event event, server_stream_ptr stream) override;
    void on_write(io_event event, server_stream_ptr stream) override;
    void read_server(int id) override;
    void write_server(int id, io_event event) override;

    void on_connect(io_event event, client_stream_ptr stream) override;
    void on_read(io_event event, client_stream_ptr stream) override;
    void on_write(io_event event, client_stream_ptr stream) override;
    void read_client(int id) override;
    void write_client(int id, io_event event) override;
    void connect(int id, std::string host, std::string service) override;

private:
    struct socks_pair {
        int id;
        server_stream_ptr server;
        client_stream_ptr client;
    };

    std::unordered_map<int, socks_pair> sessions_;
    std::unordered_map<int, socks5_session> states_;
};

using socks5_stream_manager_ptr = std::shared_ptr<socks5_stream_manager>;


#endif //SOCKS5_PROXY_SOCKS5_STREAM_MANAGER_H
