#ifndef SOCKS5_PROXY_STREAM_MANAGER_H
#define SOCKS5_PROXY_STREAM_MANAGER_H

#include "server_stream.h"
#include "client_stream.h"

class stream_manager {
public:
    virtual void start(server_stream_ptr ptr) = 0;
    virtual void stop(stream_ptr ptr) = 0;
    virtual void notify(io_event event, server_stream_ptr ptr) = 0;
    virtual void notify(io_event event, client_stream_ptr ptr) = 0;
};

using stream_manager_ptr = std::shared_ptr<stream_manager>;


#endif //SOCKS5_PROXY_STREAM_MANAGER_H
