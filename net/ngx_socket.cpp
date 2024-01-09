#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <errno.h>
#include <sys/ioctl.h>
#include <string>
#include <cstdint>
#include <unistd.h>
#include <sys/epoll.h>


#include "ngx_socket.h"
#include "ngx_c_conf.h"
#include "ngx_func.h"
#include "ngx_marco.h"







CSocket::~CSocket() {
    if(!m_listen_socket_list.empty()) {
        for(auto i : m_listen_socket_list) delete i;
    }
}

bool CSocket::initialize() {
    read_conf();
    if(ngx_open_listening_sockets())
        return true;
    else
        return false;
}

bool CSocket::ngx_open_listening_sockets() {
    CConfig* p_config = CConfig::GetInstance();
    // this -> m_listen_socket_count = stoi(p_config -> GetConfInfo("ListenPortCount"));

    int isocket;
    struct sockaddr_in serv_addr;
    int iport;

    memset(&serv_addr, 0, sizeof(sockaddr_in));
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    for(int i = 0; i < m_listen_socket_count; ++ i) {
        isocket = socket(AF_INET, SOCK_STREAM, 0);
        if(isocket == -1) {
            ngx_log_stderr(errno, "CSocket::ngx_open_listening_socket FAIL");
            return false;
        }

        int reuseaddr = 1;
        if(setsockopt(isocket, SOL_SOCKET, SO_REUSEADDR, (const void*)&reuseaddr, sizeof(reuseaddr)) == -1) {
            ngx_log_stderr(errno, "CSocket::setsocketopt FAIL");
            close(isocket);
            return false;
        }

        if(!set_noblocking(isocket)) {
            ngx_log_stderr(errno, "CSocket::set_nonblocking FAIL");
            close(isocket);
            return false;
        }

        std::string port_name = "ListenPort" + std::to_string(i);
        iport = stoi(p_config -> GetConfInfo(port_name));
        serv_addr.sin_port = htons((u_short)iport);

        if(bind(isocket, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1) {
            ngx_log_stderr(errno, "CSocket::bind FAIL");
            close(isocket);
            return false;
        }

        if(listen(isocket, NGX_LISTEN_BACKOG) == -1) {
            ngx_log_stderr(errno, "CSocket::listen FAIL");
            close(isocket);
            return false;

        }

        ngx_listen_ports_t* p_listen_socket = new ngx_listen_ports_t();
        memset(p_listen_socket, 0, sizeof(ngx_listen_ports_t));
        p_listen_socket -> port = iport;
        p_listen_socket -> fd = isocket;
        ngx_log_error_core(NGX_LOG_INFO, 0, "Listen Port%d, Success", iport);
        m_listen_socket_list.push_back(p_listen_socket);

    }
    return true;


}

void CSocket::ngx_close_listening_sockets() {
    if(!m_listen_socket_list.empty()) {
        for(auto i : m_listen_socket_list) {
            close(i -> fd);
        }
    }
}

bool CSocket::set_noblocking(int socket_fd) {
    int nb = 1;
    if(ioctl(socket_fd, FIONBIO, &nb) == -1) {
        return false;
    }
    return true;
}

void CSocket::read_conf() {
    CConfig *p_config = CConfig::GetInstance();
    m_worker_connections = stoi(p_config -> GetConfInfo("Worker_Connections"));
    m_listen_socket_count = stoi(p_config -> GetConfInfo("ListenPortCount"));
    return;

}

int CSocket::ngx_epoll_init() {
    m_epoll_handle = epoll_create(m_worker_connections);
    if(m_epoll_handle == -1) {
        ngx_log_stderr(errno, "CSocket::ngx_epoll_create FAIL");
        exit(2);
    }

    m_total_conn_n = m_worker_connections;
    m_pconnections_head = new ngx_connections_t[m_total_conn_n];

    int i = m_total_conn_n;
    ngx_connections_t* next = nullptr;
    ngx_connections_t* p_conn = m_pconnections_head;

    do {
        i --;
        p_conn[i].next = next;
        p_conn[i].fd = -1;
        p_conn[i].instance = 1;
        p_conn[i].connection_index = 0;
        
        next = &p_conn[i]; 

    }while(i);

    m_pfree_connection = m_pconnections_head;
    m_free_conn_n = m_worker_connections;

    std::vector<ngx_listen_ports_t*>::iterator pos;
    for(pos = m_listen_socket_list.begin(); pos != m_listen_socket_list.end(); ++ pos) {
        p_conn = ngx_get_connection((*pos) -> fd);
        if(!p_conn) {
            ngx_log_stderr(errno, "CSocket::ngx_epoll_init::ngx_get_connection FAIL");
            exit(2);
        }

        p_conn -> p_listen_port = (*pos);
        (*pos) -> p_connection = p_conn;

        p_conn -> read_handler = &CSocket::ngx_event_accept;

        if(ngx_epoll_add_event((*pos) -> fd, 1, 0, 0, EPOLL_CTL_ADD, p_conn) == -1) {
            ngx_log_stderr(errno, "CScoket::epoll_init::ngx_epll_add_event FAIL");
            exit(2);
        }
    }

    return 1;

}

int CSocket::ngx_epoll_add_event(int fd, int readevent, int writeevent, 
                uint32_t otherflag, uint32_t eventtype, ngx_connections_t* c) {
    struct epoll_event ev;
    memset(&ev, 0, sizeof(ev));

    if(readevent) {
        // do sth
        ev.events = EPOLLIN|EPOLLRDHUP;
    }
    else {

    }

    if(writeevent) {
        // do sth
    }
    else {
        
    }

    if(otherflag) {
        ev.events |= otherflag;
    }

    ev.data.ptr = (void*)((uintptr_t)c|c->instance);
    // 这个还不知道啥用处

    if(epoll_ctl(m_epoll_handle, eventtype, fd, &ev) == -1) {
        ngx_log_stderr(errno, "ngx_epoll_add_event FAIL");
        return -1;
    }

    return 1;
}

int CSocket::ngx_epoll_process_events(int timer) {
    int events = epoll_wait(m_epoll_handle, m_events, NGX_MAX_EPOLL_EVENTS, -1);

    if(events == -1) {
        if(errno == EINTR) {
            ngx_log_error_core(NGX_LOG_INFO, errno, "CSocket::ngx_epoll_process_events().epoll_wait() FAIL1");
            return 1;
        }
        else {
        ngx_log_error_core(NGX_LOG_ALERT, errno, "CSocket::ngx_epoll_process_events().epoll_wait() FAIL2");
        return 0;
    }
    }
    

    if(events == 0) {
        if(timer != -1) {
            return 1;
        }
        ngx_log_error_core(NGX_LOG_ALERT, 0, "CSocket::epoll_wait time out with 0 events");
        return 0;
    }

    ngx_connections_t *c;
    uintptr_t instance;
    uint32_t revents;

    for(int i = 0; i < events; ++ i) {
        c = (ngx_connections_t*)(m_events[i].data.ptr);
        // epoll有个方便的地方就是可以返回用户自定义的数据结构
        instance = (uintptr_t)c & 1;
        c = (ngx_connections_t*)((uintptr_t)c & (uintptr_t)~1);

        if(c -> fd == -1) {
            ngx_log_error_core(NGX_LOG_DEBUG, 0, "CSocket::ngx_epoll_process_events fd = -1");
            continue;
        }

        if(c -> instance != instance) {
            ngx_log_error_core(NGX_LOG_DEBUG, 0, "CSocket::ngx_epoll_process_events() instance FAIL");
            continue;
        }

        revents = m_events[i].events;
        if(revents & (EPOLLERR|EPOLLHUP)) {
            revents |= EPOLLIN | EPOLLOUT;
        }

        if(revents & EPOLLIN) {
            // (this -> * (c -> read_handler))(c);
            // ->和* 之间是否与空格决定了这条语句是否能通过编译，到低是为啥？
            (this->* (c->read_handler) )(c);

        }

        if(revents & EPOLLOUT) {
            // to do 
        }
    }
    return 1;    
}