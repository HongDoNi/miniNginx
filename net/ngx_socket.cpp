#include <iostream>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <errno.h>
#include <sys/ioctl.h>
#include <string>


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
    if(ngx_open_listening_sockets())
        return true;
    else
        return false;
}

bool CSocket::ngx_open_listening_sockets() {
    CConfig* p_config = CConfig::GetInstance();
    this -> m_listen_socket_count = stoi(p_config -> GetConfInfo("ListenPortCount"));

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

        ListenSocket* p_listen_socket = new ListenSocket();
        memset(p_listen_socket, 0, sizeof(ListenSocket));
        p_listen_socket -> port = iport;
        p_listen_socket -> listen_fd = isocket;
        ngx_log_error_core(NGX_LOG_INFO, 0, "Listen Port%d, Success", iport);
        m_listen_socket_list.push_back(p_listen_socket);

    }
    return true;


}

void CSocket::ngx_close_listening_sockets() {
    if(!m_listen_socket_list.empty()) {
        for(auto i : m_listen_socket_list) {
            close(i -> listen_fd);
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