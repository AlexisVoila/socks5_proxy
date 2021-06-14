#ifndef SOCKS5_PROXY_TCP_SERVER_STREAM_H
#define SOCKS5_PROXY_TCP_SERVER_STREAM_H

#include "server_stream.h"
#include <boost/asio.hpp>

namespace net = boost::asio;
namespace sys = boost::system;
using tcp = boost::asio::ip::tcp;

class tcp_server_stream final : public server_stream {
public:
    tcp_server_stream(const stream_manager_ptr& ptr, int id, const net::executor& ctx);
    ~tcp_server_stream() override;

    net::executor executor() override;
    tcp::socket& socket();
private:
    void do_start() final;
    void do_stop() final;
    void do_read() final;
    void do_write(io_event event) final;

    void close(const sys::error_code& ec);

    static std::string ep_to_str(const tcp::endpoint& ep);
    std::string remote_ep_str() const;
    std::string local_ep_str() const;

    tcp::socket socket_;

    std::array<std::uint8_t, max_buffer_size> read_buffer_;
    std::array<std::uint8_t, max_buffer_size> write_buffer_;
};

using server_ptr = std::shared_ptr<tcp_server_stream>;


#endif //SOCKS5_PROXY_TCP_SERVER_STREAM_H
