#include "../include/io_event.h"

io_event::io_event(int _id, io_event::event_type _type, const std::uint8_t* data, std::size_t length)
    : id(_id), type(_type), buffer(data, data + length)
{
}

io_event::io_event(int _id, io_event::event_type _type, std::vector<std::uint8_t>&& data)
    : id(_id), type(_type), buffer(move(data))
{
}
