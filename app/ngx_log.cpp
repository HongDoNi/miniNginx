#include <iostream>
#include <cstdarg>
#include <fcntl.h>
#include <unistd.h>

#include "ngx_global.h"
#include "ngx_marco.h"
#include "ngx_func.h"
#include "ngx_c_conf.h"



static u_char err_levels[][20]{
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

ngx_log_t ngx_log;

void ngx_log_stderr(int err, const char* fmt, ...) {
    va_list args;
    u_char errstr[NGX_MAX_ERR_STR_LEN + 1];

    memset(errstr, 0, sizeof(errstr));

    u_char* last = errstr + NGX_MAX_ERR_STR_LEN;
    u_char* p_cur = ngx_memcpy(errstr, "nginx: ", 7);

    va_start(args, fmt);   
    p_cur = ngx_vslprintf(p_cur, last, fmt, args);
    va_end(args);

    if(err) {
        p_cur = ngx_log_errno(p_cur, last, err);
    }

    *p_cur ++ = '\n';
    write(STDERR_FILENO,errstr,p_cur - errstr);

}


u_char*  ngx_log_errno(u_char *buf, u_char* last, int err) {
    char* perrorinfo = strerror(err);
    size_t len = strlen(perrorinfo);

    char leftstr[10] = {0};
    snprintf(leftstr, 10," (%d: ", err);
    size_t leftlen = strlen(leftstr);

    char rightstr[] = ") ";
    size_t rightlen = strlen(rightstr);

    size_t extralen = leftlen + rightlen;
    if((buf + len + extralen) < last) {
        buf = ngx_memcpy(buf, leftstr, leftlen);
        buf = ngx_memcpy(buf, perrorinfo, len);
        buf = ngx_memcpy(buf, rightstr, rightlen);
    }
    return buf;

}

void ngx_log_error_core(int level, int err, const char* fmt, ...) {
    u_char* last;
    u_char errstr[NGX_MAX_ERR_STR_LEN + 1];

    memset(errstr, 0, sizeof(errstr));
    last = errstr + NGX_MAX_ERR_STR_LEN;

    struct timeval tv;
    struct tm tm;
    time_t sec;
    u_char *p;
    va_list args;

    memset(&tv, 0, sizeof(tv));
    memset(&tm, 0, sizeof(tm));
    
    gettimeofday(&tv, nullptr);
    sec = tv.tv_sec;
    localtime_r(&sec, &tm);
    tm.tm_mon++;
    tm.tm_year += 1900;

    u_char strcurrtime[40] = {0};
    ngx_slprintf(strcurrtime,  
                    (u_char *)-1,                       //若用一个u_char *接一个 (u_char *)-1,则 得到的结果是 0xffffffff....，这个值足够大
                    "%4d/%02d/%02d %02d:%02d:%02d",     //格式是 年/月/日 时:分:秒
                    tm.tm_year, tm.tm_mon,
                    tm.tm_mday, tm.tm_hour,
                    tm.tm_min, tm.tm_sec);

    p = ngx_memcpy(errstr, strcurrtime, strlen((const char*)strcurrtime));
    p = ngx_slprintf(p, last, " [%s] ", err_levels[level]);
    p = ngx_slprintf(p, last, "%P: ", ngx_master_pid);

    va_start(args, fmt);
    p = ngx_vslprintf(p, last, fmt, args);
    va_end(args);

    if(err) {
        p = ngx_log_errno(p, last, err);
    }
    if(p >= (last -1)) {
        p = (last - 1) - 1;
    }
    *p++ = '\n';

    ssize_t n;

    while(1) {
        if(level > ngx_log.log_level) break;

        n = write(ngx_log.fd, errstr, p - errstr);

        if(n == -1) {
            if(errno == ENOSPC) {
                
            }
            else {
                if(ngx_log.fd != STDERR_FILENO) {
                    n = write(STDERR_FILENO, errstr, p - errstr);
                }
            }
        }
        break;
    }
    return;
}

void ngx_log_init() {
    std::string plogname{""};
    size_t nlen;

    CConfig *p_config = CConfig::GetInstance();
    plogname = p_config->GetConfInfo("Log");
    if(plogname == "") {
        plogname = "logs/error1.log";
    }
    ngx_log.log_level = stoi(p_config -> GetConfInfo("LogLevel"));

    ngx_log.fd = open((plogname).c_str(), O_WRONLY|O_APPEND|O_CREAT, 0644);

    if(ngx_log.fd == -1) {
        ngx_log_stderr(errno, "[alert] could not open error log file: open() \"%s\" failed", plogname.c_str());
        ngx_log.fd = STDERR_FILENO;
    }
    return;
}

