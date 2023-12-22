#ifndef MININGINX_NGX_FUNC_H
#define MININGINX_NGX_FUNC_H
#include <iostream>
#include <cstring>



void Rtrim(std::string&);
void Ltrim(std::string&);

void ngx_init_setproctitle();
void ngx_setproctitle(const char*);

u_char* ngx_slprintf(u_char* buf, u_char* last, const char* fmt, ...);
u_char* ngx_vslprintf(u_char*, u_char*, const char*, va_list);

void ngx_log_stderr(int, const char*, ...);

#endif //MININGINX_NGX_FUNC_H
