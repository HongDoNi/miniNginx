#include <cstring>

#include "ngx_func.h"
#include "ngx_global.h"

void ngx_init_setproctitle() {

    for(int i = 0; environ[i]; ++ i) {
        g_environ_len += strlen(environ[i]) + 1;
    }

    char* g_new_environ = new char[g_environ_len];
    memset(g_new_environ, 0, g_environ_len);
    
    char* ptmp = g_new_environ;
    for(int i = 0; environ[i]; ++ i) {

        memcpy(ptmp,environ[i], strlen(environ[i]) + 1);
        environ[i] = ptmp;
        ptmp += strlen(environ[i]) + 1;
    }
}

void ngx_setproctitle(const char* title) {
    size_t title_len = strlen(title);
    size_t argvs_len = 0;
    
    for(int i = 0; g_os_argv[i]; ++ i) {
        argvs_len += strlen(g_os_argv[i]) + 1; 
    }

    size_t total_len = argvs_len + g_environ_len;

    if(title_len > total_len) return;

    char* ptmp = g_os_argv[0];
    memcpy(ptmp, title, title_len);
    ptmp +=  title_len;
    memset(ptmp, 0, total_len - title_len);
}
