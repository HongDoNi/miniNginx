#ifndef MININGINX_NGX_FUNC_H
#define MININGINX_NGX_FUNC_H
#include <iostream>

void Rtrim(std::string&);
void Ltrim(std::string&);

void ngx_init_setproctitle();
void ngx_setproctitle(const char*);

#endif //MININGINX_NGX_FUNC_H
