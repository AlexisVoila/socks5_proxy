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

    void start(server_stream_ptr stream) override;
    void stop(stream_ptr stream) override;

    void notify(io_event event, server_stream_ptr stream) override;
    void notify(io_event event, client_stream_ptr stream) override;
private:
    std::unordered_map<int, socks5_session> sessions_;
};

using socks5_stream_manager_ptr = std::shared_ptr<socks5_stream_manager>;


#endif //SOCKS5_PROXY_SOCKS5_STREAM_MANAGER_H
