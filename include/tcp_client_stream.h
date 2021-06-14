#ifndef SOCKS5_PROXY_TCP_CLIENT_STREAM_H
#define SOCKS5_PROXY_TCP_CLIENT_STREAM_H

#include "client_stream.h"
#include <boost/asio.hpp>

namespace net = boost::asio;
namespace sys = boost::system;
using tcp = boost::asio::ip::tcp;

class tcp_client_stream final : public client_stream {
public:
    tcp_client_stream(const stream_manager_ptr& ptr, int id, const net::executor& ctx);
    ~tcp_client_stream() override;

private:
    void do_start() final;
    void do_stop() final;

    void do_connect(const tcp::resolver::results_type& ep_iter);

    void do_read() final;
    void do_write(io_event event) final;

    void close(const sys::error_code& ec);

    void do_set_host(const std::string& host) final;
    void do_set_service(const std::string& service) final;

    static std::string ep_to_str(const tcp::endpoint& ep);
    std::string remote_ep_str() const;
    std::string local_ep_str() const;

    tcp::socket socket_;
    tcp::resolver resolver_;

    std::string host_;
    std::string port_;

    std::array<std::uint8_t, max_buffer_size> read_buffer_;
    std::array<std::uint8_t, max_buffer_size> write_buffer_;
};

#endif //SOCKS5_PROXY_TCP_CLIENT_STREAM_H
