// #include "ngx_socket.h"
#include "ngx_global.h"
#include "ngx_func.h"


void ngx_process_events_and_timers() {
    g_socket.ngx_epoll_process_events(-1);
}


