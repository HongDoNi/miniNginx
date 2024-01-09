#include <unistd.h>

#include "ngx_socket.h"
#include "ngx_func.h"
#include "ngx_c_conf.h"
#include "ngx_global.h"

void CSocket::ngx_event_accept(ngx_connections_t* old_c) {
    // do sth

    // ngx_log_stderr(0, "doing ngx_event_accept handler");
    ngx_log_error_core(0,0, "doing ngx_event_accept handler");
    struct sockaddr peer_sockaddr;
    socklen_t socklen;
    int err;
    int level;
    int s;
    static int use_accept4 = 1;
    ngx_connections_t *new_c;

    socklen = sizeof(socklen);

    do {
        if(use_accept4) {
            s = accept4(old_c -> fd,&peer_sockaddr, &socklen, SOCK_NONBLOCK);
        }
        else {
            s = accept(old_c -> fd, &peer_sockaddr, &socklen);
        }
        if(s == -1) {
            err = errno;
            if(err == EAGAIN || err == EWOULDBLOCK) {
                return;
            }
            level = NGX_LOG_ALERT;
            if(err == ECONNABORTED) {
                level = NGX_LOG_ERR;
            }
            else if (err == EMFILE || err == ENFILE) {
                level = NGX_LOG_CRIT;
            }
            ngx_log_error_core(level, errno, "CSocket::ngx_event_accept accept() FAIL");

            if(use_accept4 && err == ENOSYS) {
                use_accept4 = 0;
                continue;
            }

            if(err == ECONNABORTED) {

            }

            if(err == EMFILE || err == ENFILE) {

            }
            return;
        }

        new_c = ngx_get_connection(s);

        if(new_c == nullptr) {
            if(close(s) == -1) {
                ngx_log_error_core(NGX_LOG_ALERT, errno, "CSocket::ngx_event_accept close() FAIL");
            }
        }

        memcpy(&(new_c -> client_addr), &peer_sockaddr, socklen);
        {
           //测试将收到的地址弄成字符串，格式形如"192.168.1.126:40904"或者"192.168.1.126"
           u_char ipaddr[100]; memset(ipaddr,0,sizeof(ipaddr));
           ngx_sock_ntop(&new_c->client_addr,1,ipaddr,sizeof(ipaddr)-10); //宽度给小点
           ngx_log_stderr(0,"ip信息为%s\n",ipaddr);
        }

        if(!use_accept4) {
            if(set_noblocking(s) == false) {
                ngx_close_connection(new_c);
                return;
            }
        }

        new_c -> p_listen_port = old_c -> p_listen_port;
        new_c -> write_ready = 1;
        new_c -> read_handler = &CSocket::ngx_wait_request_handler;

        if(ngx_epoll_add_event(s, 1, 0, EPOLLET, EPOLL_CTL_ADD, new_c) != 1) {
            ngx_close_connection(new_c);
            return;
        }
        break;
    }while(1);
    
    return;
   
}


