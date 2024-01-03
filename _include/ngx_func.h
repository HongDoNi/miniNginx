#ifndef MININGINX_NGX_FUNC_H
#define MININGINX_NGX_FUNC_H
#include <iostream>
#include <cstring>
#include <cstdarg>
#include <sys/time.h>
// #include <unistd.h>
#include <sys/stat.h>
#include <signal.h>
#include <fcntl.h>


void Rtrim(std::string&);
void Ltrim(std::string&);

void ngx_init_setproctitle();
void ngx_setproctitle(const char*);

u_char* ngx_slprintf(u_char* buf, u_char* last, const char* fmt, ...);
u_char* ngx_vslprintf(u_char*, u_char*, const char*, va_list);

void ngx_log_stderr(int, const char*, ...);
u_char* ngx_log_errno(u_char* , u_char* , int);
void ngx_log_error_core(int , int, const char*, ...);
void ngx_log_init();

int ngx_init_signals();

int ngx_enable_daemon();
void ngx_master_process_cycle();

#endif //MININGINX_NGX_FUNC_H
