#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>

#include "ngx_socket.h"
#include "ngx_func.h"

void CSocket::ngx_wait_request_handler(ngx_connections_t* c) {
    ngx_log_error_core(0, 0, "doing ngx_wait_request_handler");

}


// 