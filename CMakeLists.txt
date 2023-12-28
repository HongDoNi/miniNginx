cmake_minimum_required(VERSION 3.26)
project(miniNginx)

set(CMAKE_CXX_STANDARD 23)

include_directories(${PROJECT_SOURCE_DIR}/_include)

add_executable(miniNginx app/miniNginx.cpp
        _include/ngx_signal.h
        app/ngx_setproctitle.cpp
        signal/ngx_signal.cpp
        app/ngx_c_conf.cpp
        _include/ngx_global.h
        _include/ngx_c_conf.h
        _include/ngx_func.h
        _include/ngx_marco.h
        _include/ngx_socket.h
        app/ngx_string.cpp
        app/ngx_log.cpp
        app/ngx_printf.cpp
        proc/ngx_process_cycle.cpp
        proc/ngx_daemon.cpp
        net/ngx_socket.cpp
        net/ngx_socket.cpp)
