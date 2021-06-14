cmake_minimum_required(VERSION 3.17)
project(socks5_proxy)

set(CMAKE_CXX_STANDARD 17)
#set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O0")
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0")

#DEBUG|RELEASE|RELWITHDEBINFO|MINSIZEREL
#set( CMAKE_BUILD_TYPE RELEASE )

find_package(Boost REQUIRED COMPONENTS thread program_options)

include_directories(${Boost_INCLUDE_DIR})
include_directories(${PROJECT_SOURCE_DIR}/include)

set (HEADERS
        include/stream.h
        include/client_stream.h
        include/server_stream.h
        include/stream_manager.h
        include/socks5_stream_manager.h
        include/tcp_server_stream.h
        include/tcp_client_stream.h
        include/server.h
        include/socks5.h
        include/socks5_state.h
        include/socks5_session.h
        include/io_event.h
        logger/logger.h)

set (SOURCES
        src/socks5_stream_manager.cpp
        src/server.cpp
        src/tcp_server_stream.cpp
        src/tcp_client_stream.cpp
        src/socks5_state.cpp
        src/socks5_session.cpp
        src/io_event.cpp
        src/socks5.cpp
        logger/logger.cpp
        main.cpp)

add_executable(socks5_proxy ${SOURCES} ${HEADERS})
target_link_libraries(${PROJECT_NAME} ${Boost_LIBRARIES})
