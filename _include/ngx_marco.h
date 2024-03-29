#ifndef MININGINX_NGX_MARCO_H
#define MININGINX_NGX_MARCO_H

#include <cstring>

#define NGX_MAX_ERR_STR_LEN 2048

#define NGX_LISTEN_BACKOG 511
#define NGX_MAX_EPOLL_EVENTS 512

#define ngx_memcpy(dst, src, n) (((u_char*)memcpy(dst, src, n)) + (n))

#define NGX_LOG_STDERR            0    //控制台错误【stderr】：最高级别日志，日志的内容不再写入log参数指定的文件，而是会直接将日志输出到标准错误设备比如控制台屏幕
#define NGX_LOG_EMERG             1    //紧急 【emerg】
#define NGX_LOG_ALERT             2    //警戒 【alert】
#define NGX_LOG_CRIT              3    //严重 【crit】
#define NGX_LOG_ERR               4    //错误 【error】：属于常用级别
#define NGX_LOG_WARN              5    //警告 【warn】：属于常用级别
#define NGX_LOG_NOTICE            6    //注意 【notice】
#define NGX_LOG_INFO              7    //信息 【info】
#define NGX_LOG_DEBUG             8    //调试 【debug】：最低级别

#define NGX_MASTER_PROCESS 0
#define NGX_WORKER_PROCESS 1

#define NGX_ERROR_LOG_PATH       "logs/error1.log"   //定义日志存放的路径和文件名

#define _PKG_MAX_LENGTH 30000

#define _PKG_HD_INIT 0
#define _PKG_HD_RECVING 1
#define _PKG_BD_INIT 2
#define _PKG_BD_RECVING 3

#define _DATA_BUFSIZE_ 20 // 用来存包头的缓冲区


#endif
