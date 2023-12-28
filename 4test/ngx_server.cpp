#include <iostream>
#include <sys/socket.h>
#include <sys/types.h>
#include <arpa/inet.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>

#define server_port 8081

int main() {
    int server_socket_fd = socket(AF_INET, SOCK_STREAM, 0);

    struct sockaddr_in serv_addr;
    memset(&serv_addr, 0, sizeof(serv_addr);

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(server_port);
    serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);

    int reuseaddr = 1;
    if(setsockopt(server_socket_fd, SOL_SOCKET, SO_REUSEADDR, (const void*)&reuseaddr, sizeof(reuseaddr)) == -1) {
        char *perrorinfo = strerror(errno);
        printf("setsockopt error \n");
    }

    int result;
    result = bind(server_socket_fd, (struct sockaddr*)&serv_addr, sizeof(serv_addr));
    if(result == -1) {
        printf("bind error\n");
        return -1;
    }

    result = listen(server_socket_fd, 32);
    if(result = -1) {
        printf("listen error\n");
        return -1;
    }

    int connfd;
    const char *pcontent = "i sent sth to client!\n";

    while(1) {
        connfd = accept(server_socket_fd, (struct sockaddr*)nullptr, nullptr);
        write(connfd, pcontent, strlen(pcontent));
        
        close(connfd);
    }

    return 0;


}