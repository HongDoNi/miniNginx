#include <cstring>

#include "ngx_func.h"
#include "ngx_global.h"

void ngx_init_setproctitle() {
    size_t environ_len = 0;
    for(int i = 0; environ[i]; ++ i) {
        environ_len += strlen(environ[i]) + 1;
    }

    char* new_environ = new char[environ_len];
    memset(new_environ, 0, environ_len);
    
    char* ptmp = new_environ;
    for(int i = 0; environ[i]; ++ i) {

        memcpy(ptmp,environ[i], strlen(environ[i]) + 1);
        environ[i] = ptmp;
        ptmp += strlen(environ[i]) + 1;
    }

}

void ngx_setproctitle(const std::string title) {

}
