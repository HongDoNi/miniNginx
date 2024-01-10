#pragma once

#include "ngx_socket.h"



#pragma pack(1)

typedef struct _COMM_PKG_HEADER {
    unsigned short pkg_length;
    unsigned short pkg_msgCode; // 消息类型代码
    int pkg_crc32;
}COMM_PKG_HEADER;

typedef struct _COMM_MSG_HEADER {
    ngx_connections_t* p_connection;
    uint64_t connection_index;
}COMM_MSG_HEADER;



