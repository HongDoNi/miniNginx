#pragma once

#include <vector>
#include <sys/epoll.h>

struct ngx_listen_ports_t;
struct ngx_connections_t;

typedef void (*ngx_event_handler_pt)(ngx_connections_t *c);

struct ngx_listen_ports_t{
    int port;
    int fd;
    ngx_connections_t *p_connection;
};

struct ngx_connections_t{
    int fd; 
    // 这里是叫连接套接字吗？连接套接字是在监听套接字之后通过accept生成的
    // connections_t 中的fd和listen_ports_t中的fd应该是同一个东西把？

    ngx_listen_ports_t *p_listen_port;

    uint64_t connection_index;  // 

    ngx_event_handler_pt write_handler; // 写事件处理函数
    ngx_event_handler_pt read_handler; 

    ngx_connections_t *next;  // 所有的连接信息保存在一段连续的内存中，next指向下一个连接信息地址块

};


class CSocket {
public:
    int m_listen_socket_count;
    int m_worker_connections;
    int m_epoll_handle;

    ngx_connections_t* m_pconnections_head;
    ngx_connections_t* m_pfree_connection;
    int m_total_conn_n; // 连接池中总可支持的连接总数
    int m_free_conn_n; // 连接池中空闲的连接数

    std::vector<ngx_listen_ports_t*> m_listen_socket_list;

public:
    CSocket() = default;

    ~CSocket();

public:
    bool initialize();

public:
    int ngx_epoll_init();
    int ngx_epoll_add_event(int fd, int readevent, int writeevent, uint32_t otherflag, uint32_t eventtype, ngx_connections_t* c);
    int ngx_epoll_process_events(int timer);

public:

    // handlers
    void ngx_event_accept(ngx_connections_t*);

private:
    bool ngx_open_listening_sockets();
    void ngx_close_listening_sockets();
    bool set_noblocking(int socket_fd);
    ngx_connections_t* ngx_get_connection(int);
    void read_conf();

};