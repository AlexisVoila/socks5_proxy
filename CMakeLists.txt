cmake_minimum_required(VERSION 3.17)
project(socks5_proxy)

set(CMAKE_CXX_STANDARD 20)

# Suppressing relish warnings when using fresh versions of C++ Boost
set(Boost_NO_WARN_NEW_VERSIONS 1)

if (WIN32)
    set(Boost_USE_STATIC_LIBS       ON)
    set(Boost_USE_MULTITHREADED     ON)
    set(Boost_USE_STATIC_RUNTIME    OFF)

    set (BOOST_ROOT         "c:/dev/lib/boost/boost_1_79_0")
    set (Boost_INCLUDE_DIR  "c:/dev/lib/boost/boost_1_79_0")
    set (Boost_LIBRARIES    "c:/dev/lib/boost/boost_1_79_0/lib64-msvc-14.3")
endif()

find_package(Boost COMPONENTS program_options REQUIRED)

message(STATUS "Boost path:             ${BOOST_ROOT}")
message(STATUS "Boost include path:     ${Boost_INCLUDE_DIR}")
message(STATUS "Boost libraries path:   ${Boost_LIBRARIES}")

include_directories(${Boost_INCLUDE_DIR})
include_directories(${PROJECT_SOURCE_DIR}/include)

# Asking Boost for information about automatic link requests
add_definitions(${Boost_LIB_DIAGNOSTIC_DEFINITIONS})
set(CMAKE_VS_JUST_MY_CODE_DEBUGGING ON)

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
        src/socks5.cpp
        logger/logger.cpp
        main.cpp)

add_executable(socks5_proxy ${SOURCES} ${HEADERS})
target_link_libraries(${PROJECT_NAME} ${Boost_LIBRARIES})

if (MSVC)
    target_compile_definitions(${PROJECT_NAME} PRIVATE _WIN32_WINNT=0x0A00)
    set(CMAKE_VS_JUST_MY_CODE_DEBUGGING ON)
endif()

