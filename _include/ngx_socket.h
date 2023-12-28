#pragma once

#include <vector>

typedef struct {
    int port;
    int listen_fd;
}ListenSocket;

class CSocket {
public:
    int m_listen_socket_count;
    std::vector<ListenSocket*> m_listen_socket_list;


public:
    CSocket() = default;

    ~CSocket();

public:
    bool initialize();

private:
    bool ngx_open_listening_sockets();
    void ngx_close_listening_sockets();
    bool set_noblocking(int socket_fd);

};