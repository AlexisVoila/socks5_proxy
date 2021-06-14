#ifndef SOCKS5_PROXY_CLIENT_STREAM_H
#define SOCKS5_PROXY_CLIENT_STREAM_H


#include "stream.h"

class client_stream : public stream, public std::enable_shared_from_this<client_stream> {
public:
    client_stream(const stream_manager_ptr& smp, int id) : stream(smp, id) {}

    void set_host(const std::string& host) { do_set_host(host); }
    void set_service(const std::string& service) { do_set_service(service); }

private:
    virtual void do_set_host(const std::string& host) = 0;
    virtual void do_set_service(const std::string& service) = 0;
};

using client_stream_ptr = std::shared_ptr<client_stream>;



#endif //SOCKS5_PROXY_CLIENT_STREAM_H
