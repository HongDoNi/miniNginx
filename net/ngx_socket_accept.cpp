#include "ngx_socket.h"
#include "ngx_func.h"
#include "ngx_c_conf.h"
#include "ngx_global.h"

void CSocket::ngx_event_accept(ngx_connections_t* oldc) {
    // do sth

    ngx_log_stderr(0, "doing ngx_event_accept handler");
    
}