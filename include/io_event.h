#ifndef SOCKS5_PROXY_IO_EVENT_H
#define SOCKS5_PROXY_IO_EVENT_H

#include <cstdint>
#include <vector>

struct io_event {
    enum event_type : int {
        none,
        read,
        write,
        connect,
        close,
        error
    };

    explicit io_event(int _id, event_type _type = none, const std::uint8_t* data = nullptr, std::size_t length = 0);
    io_event(int _id, event_type _type, std::vector<std::uint8_t>&& data);

    io_event(const io_event& other) = default;
    io_event(io_event&& other) = default;
    io_event& operator=(io_event&& other) = default;

    int id;
    event_type type;
    std::vector<std::uint8_t> buffer;
};

#endif //SOCKS5_PROXY_IO_EVENT_H
