#include "ngx_socket.h"
#include "ngx_func.h"
#include "ngx_global.h"
#include "ngx_marco.h"


ngx_connections_t* CSocket::ngx_get_connection(int fd) {
    ngx_connections_t* c = m_pfree_connection;
    
    if(!c) {
        ngx_log_stderr(0, "CSocket::ngx_get_connection FAIL");
        return nullptr;
    }

    m_pfree_connection = c -> next;
    m_free_conn_n --;

    uint64_t index = c -> connection_index;

    memset(c, 0, sizeof(ngx_connections_t));
    c -> fd = fd;

    c -> connection_index = index;
    c -> connection_index ++;

    return c;
}