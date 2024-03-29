#include <unistd.h>

#include "ngx_socket.h"
#include "ngx_func.h"
#include "ngx_global.h"
#include "ngx_marco.h"
#include "ngx_comm.h"




ngx_connections_t* CSocket::ngx_get_connection(int fd) {
    // 从连接池中拿出一个空闲的连接
    ngx_connections_t* c = m_pfree_connection;
    
    if(!c) {
        ngx_log_stderr(0, "CSocket::ngx_get_connection FAIL");
        return nullptr;
    }

    m_pfree_connection = c -> next;
    m_free_conn_n --;

    uint64_t index = c -> connection_index;
    uintptr_t instance = c -> instance;

    memset(c, 0, sizeof(ngx_connections_t));
    c -> fd = fd;

    c -> curStat = _PKG_HD_INIT;
    c -> precvbuf =  c -> data_head;
    c -> irecvlen = sizeof(COMM_PKG_HEADER);
    c -> if_new_mem = false;
    c -> p_new_mem = nullptr;

    c -> instance  = !instance;
    c -> connection_index = index;
    c -> connection_index ++;
    
    return c;
}

void CSocket::ngx_free_connection(ngx_connections_t *c) {
    if(c -> if_new_mem) {
        // to do 
    }



    c -> next = m_pfree_connection;
    ++ c -> connection_index;

    m_pfree_connection = c;
    ++m_free_conn_n;
    return;

    // free connection之后，原来申请的那段内存中各个ngx_connection_t之间的链表顺序是会打乱的，但是这不影响这段内存的释放，毕竟我是知道这段内存的头地址的

}


void CSocket::ngx_close_connection(ngx_connections_t* c) {
    
    if(close(c->fd) == -1) {
        ngx_log_error_core(NGX_LOG_ALERT, errno, "CSocket::ngx_close_connection close() FAIL");
    }
    c -> fd = -1;
    ngx_free_connection(c);
    return;
}
