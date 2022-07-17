#ifndef SOCKS5_PROXY_TCP_CLIENT_STREAM_H
#define SOCKS5_PROXY_TCP_CLIENT_STREAM_H

#include "client_stream.h"
#include <boost/asio.hpp>

namespace net = boost::asio;
namespace sys = boost::system;
using tcp = boost::asio::ip::tcp;

class tcp_client_stream final : public client_stream {
public:
    tcp_client_stream(const stream_manager_ptr& ptr, int id, net::io_context& ctx);
    ~tcp_client_stream() override;

private:
    void do_start() final;
    void do_stop() final;

    void do_connect(tcp::resolver::results_type&& results);

    void do_read() final;
    void do_write(io_event event) final;

    void close(const sys::error_code& ec);

    void do_set_host(std::string host) final;
    void do_set_service(std::string service) final;

    tcp::socket socket_;
    tcp::resolver resolver_;

    std::string host_;
    std::string port_;

    std::array<std::uint8_t, max_buffer_size> read_buffer_;
    std::array<std::uint8_t, max_buffer_size> write_buffer_;
};

#endif //SOCKS5_PROXY_TCP_CLIENT_STREAM_H
