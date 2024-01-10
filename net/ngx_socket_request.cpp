#include <arpa/inet.h>
#include <sys/socket.h>
#include <unistd.h>

// #include "ngx_socket.h"
#include "ngx_func.h"
// #include "ngx_marco.h"
#include "ngx_memory.h"
#include "ngx_comm.h"



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
        if(reco == c -> irecvlen) {
            ngx_wait_request_handler_proc_plast(c);
        }
        else {
            c -> curStat = _PKG_BD_RECVING;
            c -> precvbuf += reco;
            c -> irecvlen -= reco;
        }
    }
    else if(c -> curStat == _PKG_BD_RECVING) {
        if(c -> irecvlen == reco) {
            ngx_wait_request_handler_proc_plast(c);
        }
        else {
            c -> precvbuf += reco;
            c -> irecvlen -= reco;
        }
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
    CMemory *p_memory = CMemory::get_instance();

    COMM_PKG_HEADER* p_pkg_header;
    p_pkg_header = (COMM_PKG_HEADER*)c -> data_head;

    unsigned short e_pkg_len = p_pkg_header -> pkg_length;
    e_pkg_len = ntohs(e_pkg_len);



    if(e_pkg_len < m_pkg_head_len) {
        // 处理非法包，包的长度比包头长度还短的非法包
        // 但是没想通这样做的作用

        c -> curStat = _PKG_HD_INIT;
        c -> precvbuf = c -> data_head;
        c -> irecvlen = m_pkg_head_len;
    }
    else if (e_pkg_len > (_PKG_MAX_LENGTH - 1000)) {
        // 同样的问题，为啥重置这些就可以处理非法包
        c -> curStat = _PKG_HD_INIT;
        c -> precvbuf = c -> data_head;
        c -> irecvlen = m_pkg_head_len;
    }
    else {
        char *p_temp_buff = (char *)(p_memory -> alloc_memory(m_pkg_msg_len + e_pkg_len, false));
        c -> if_new_mem = true;
        c -> p_new_mem = p_temp_buff;

        COMM_MSG_HEADER*  p_msg_header = (COMM_MSG_HEADER*)(p_temp_buff);
        p_msg_header -> p_connection = c;
        p_msg_header -> connection_index = c -> connection_index;

        p_temp_buff += m_pkg_msg_len;
        // p_temp_buff = (COMM_PKG_HEADER*)p_temp_buff;
        memcpy(p_temp_buff, p_pkg_header, m_pkg_head_len);
        if(e_pkg_len == m_pkg_head_len) {
            // 该包只有包头，无内容
            ngx_wait_request_handler_proc_plast(c);
        }
        else {
            c -> curStat = _PKG_BD_INIT;
            c -> precvbuf = p_temp_buff + m_pkg_head_len;
            c -> irecvlen = e_pkg_len - m_pkg_head_len;
        }
    }
    
    return;

}


void CSocket::ngx_wait_request_handler_proc_plast(ngx_connections_t* c) {
    // todo 把收到的内容放到消息队列中
    in_to_msg_recv_queue(c -> p_new_mem);

    c -> if_new_mem = false;
    c -> p_new_mem = nullptr;
    c -> curStat = _PKG_HD_INIT;
    c -> precvbuf = c -> data_head;
    c -> irecvlen = m_pkg_head_len;
    return;

}

void CSocket::in_to_msg_recv_queue(char* msg) {
    m_msg_recv_queue.push_back(msg);

}
void CSocket::out_from_msg_recv_queue() {
    if(!m_msg_recv_queue.empty()) {
        int msg_list_size = m_msg_recv_queue.size();
        if(msg_list_size < 1000) return;
        else {
            CMemory* p_memory = CMemory::get_instance();
            for(int i = 0; i < msg_list_size - 500; ++ i) {
                char* item = m_msg_recv_queue.front();
                m_msg_recv_queue.pop_front();
                p_memory -> free_memory(item);

            }
        }
    }
    return;
}
void CSocket::clean_msg_recv_queue() {
    CMemory* p_memory = CMemory::get_instance();

    for(auto item : m_msg_recv_queue) {
        p_memory -> free_memory(item);
    }
    m_msg_recv_queue.clear();
}