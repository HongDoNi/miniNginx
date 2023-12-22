#include <iostream>
#include <cstdarg>


#include "ngx_global.h"
#include "ngx_marco.h"
#include "ngx_func.h"

static u_char log_levels[][20]{
    "stderr",
    "emerg",
    "alert",
    "crit",
    "err",
    "warn",
    "notice",
    "info",
    "debug"
};

void ngx_log_stderr(int err, const char* fmt, ...) {
    va_list args;
    u_char errstr[NGX_MAX_ERR_STR_LEN + 1];

    memset(errstr, 0, sizeof(errstr));

    u_char* last = errstr + NGX_MAX_ERR_STR_LEN;
    u_char* p_cur = ngx_memcpy(errstr, "nginx: ", 7);

    va_start(args, fmt);   
    p_cur = ngx_vslprintf(p_cur, last, fmt, args);
    va_end(args);

    // if(err) {
    //     p = 
    // }

    *p_cur ++ = '\n';


}

