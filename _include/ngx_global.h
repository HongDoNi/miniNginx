

#ifndef MININGINX_NGX_GLOBAL_H
#define MININGINX_NGX_GLOBAL_H


extern char* g_new_environ;
//extern char** environ;
extern char** g_os_argv;
extern size_t g_environ_len;
extern pid_t ngx_pid;

typedef unsigned char u_char;

typedef struct {
    int log_level;
    int fd;
}ngx_log_t;


#endif //MININGINX_NGX_GLOBAL_H
