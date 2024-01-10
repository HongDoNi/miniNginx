

#ifndef MININGINX_NGX_GLOBAL_H
#define MININGINX_NGX_GLOBAL_H


#include <csignal>
#include "ngx_socket.h"
#include "ngx_threadpool.h"

extern char* g_new_environ;
extern char** environ;
extern char** g_os_argv;
extern size_t g_environ_len;
extern pid_t ngx_master_pid;
extern bool g_enable_daemon;
extern int ngx_process;
extern sig_atomic_t ngx_reap;
extern CSocket g_socket;

extern CThreadPool g_threadpool;

typedef unsigned char u_char;

typedef struct {
    int log_level;
    int fd;
}ngx_log_t;

extern ngx_log_t ngx_log;


#endif //MININGINX_NGX_GLOBAL_H
