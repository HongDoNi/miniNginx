cmake_minimum_required(VERSION 3.26)
project(miniNginx)

set(CMAKE_CXX_STANDARD 23)

add_executable(miniNginx app/miniNginx.cpp
        _include/ngx_signal.h
        app/ngx_setproctitle.cpp
        signal/ngx_signal.cpp
        app/ngx_c_config.cpp
        _include/ngx_global.h)
