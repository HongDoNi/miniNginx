#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>

#include "ngx_socket.h"
#include "ngx_func.h"
#include "ngx_marco.h"

void CSocket::ngx_wait_request_handler(ngx_connections_t* c) {
    ngx_log_error_core(0, 0, "doing ngx_wait_request_handler");
    ssize_t reco = recvproc(c, c -> precvbuf, c -> irecvlen);

    if(reco <= 0) return;

    if(c -> curStat == _PKG_HD_INIT) {
        if(reco == m_pkg_head_len) {
            ngx_wait_request_handler_proc_p1(c);
        }
        else {
            c -> curStat = _PKG_HD_RECVING;
            c -> precvbuf += reco;
            c -> irecvlen -= reco;
        }
    }
    else if(c -> curStat == _PKG_HD_RECVING) {
        if(reco == c -> irecvlen) {
            ngx_wait_request_handler_proc_p1(c);
        }
        else {
            c -> precvbuf += reco;
            c -> irecvlen -= reco;
        }
    }
    else if(c -> curStat == _PKG_BD_INIT) {
        
    }
    else if(c -> curStat == _PKG_BD_RECVING) {

    }

    return;
    

}


// 

ssize_t CSocket::recvproc(ngx_connections_t* c, char *buff, ssize_t buflen) {
    ssize_t n;

    n = recv(c -> fd, buff, buflen, 0);
    if(n == 0) {
        ngx_close_connection(c);
        return -1;
    }

    if(n < 0) {
        if(errno == EAGAIN || errno == EWOULDBLOCK) {
            ngx_log_error_core(NGX_LOG_ERR, errno, "CSocket::recvproc occur EAGAIN or EWOULDBLOCK");
            return -1;
        }

        if(errno == EINTR) {
            ngx_log_error_core(NGX_LOG_ERR, errno, "CSocket::recvproc occur EINTR");
            return 01;
        }

        if(errno == ECONNRESET) {
            // 客户端非正常通过四挥手关闭连接，而是暴力关闭连接，那么服务端会收到RST包，从而errno为ECONNRESET
        }
        else {
            ngx_log_error_core(NGX_LOG_ERR, errno, "CSocket::recvproc occur other errors");
        }

        ngx_close_connection(c);
        return -1;
    }

    return n;

}


void CSocket::ngx_wait_request_handler_proc_p1(ngx_connections_t* c) {

}


void CSocket::ngx_wait_request_handler_proc_plast(ngx_connections_t* c) {

}