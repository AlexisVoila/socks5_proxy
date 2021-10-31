#include "../include/socks5_stream_manager.h"
#include "../include/tcp_client_stream.h"
#include "../logger/logger.h"
#include <boost/format.hpp>

using fmt = boost::format;
using logger = logging::logger;

void socks5_stream_manager::start(server_stream_ptr stream) {
    stream->start();

    logger::trace((fmt("[%1%] session created") % stream->id()).str());

    auto cli_stream = std::make_shared<tcp_client_stream>(shared_from_this(), stream->id(), stream->context());

    socks5_session session{socks5_session(stream->id(), stream, std::move(cli_stream))};
    sessions_.insert({stream->id(), std::move(session)});
}

void socks5_stream_manager::stop(stream_ptr stream) {
    auto it = sessions_.find(stream->id());
    if (it != sessions_.end()) {
        it->second.stop();
        logger::info((fmt("[%1%] session closed, rx_bytes: %2%, tx_bytes: %3%")
                      % stream->id()
                      % it->second.context().transferred_bytes_to_local
                      % it->second.context().transferred_bytes_to_remote).str());
        logger::trace((fmt("live sessions %1%") % sessions_.size() ).str());
        sessions_.erase(it);
    }
}

void socks5_stream_manager::notify(io_event event, server_stream_ptr stream) {
    if (event.type == io_event::read || event.type == io_event::write) {
        auto it = sessions_.find(stream->id());
        if (it != sessions_.end())
            it->second.handle(event, stream);
    } else if (event.type == io_event::close) {
        stop(stream);
    } else {
        std::string error_string(event.buffer.begin(), event.buffer.end());
        logger::error((fmt("[%1%] session error: %2%") % stream->id() % error_string).str());
        stop(stream);
    }
}

void socks5_stream_manager::notify(io_event event, client_stream_ptr stream) {
    if (event.type == io_event::read || event.type == io_event::write || event.type == io_event::connect) {
        auto it = sessions_.find(stream->id());
        if (it != sessions_.end())
            it->second.handle(event, stream);
    } else if (event.type == io_event::close) {
        stop(stream);
    } else {
        std::string error_string(event.buffer.begin(), event.buffer.end());
        logger::error((fmt("[%1%] session error: %2%") % stream->id() % error_string).str());
        stop(stream);
    }
}
