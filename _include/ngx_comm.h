#pragma once

#define _PKG_MAX_LENGTH 30000

#define _PKG_HD_INIT 0
#define _PKG_HD_RECVING 1
#define _PKG_BD_INIT 2
#define _PKG_BD_RECVING 3

#define _DATA_BUFSIZE_ 20 // 用来存包头的缓冲区

#pragma pack(1)

typedef struct _COMM_PKG_HEADER {
    unsigned short pkg_lenth;
    unsigned short pkg_msgCode; // 消息类型代码
    int pkg_crc32;
}COMM_PKG_HEADER;

typedef struct _COMM_MSG_HEADER {
    
}COMM_MSG_HEADER;



