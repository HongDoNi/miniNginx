#include <iostream>
#include <sys/stat.h>
#include <unistd.h>
#include <stdlib.h>

#include "ngx_c_conf.h"
#include "ngx_func.h"
#include "ngx_global.h"
#include "ngx_marco.h"


int ngx_enable_daemon() {
    
    int pid = fork();
    if(pid == -1) {
        ngx_log_stderr(0, 0, "ngx_enable_daemon().fork() FAIL");
        return -1;
    }
    else if(pid > 0) {
        // parent
        return 1;
    }
    else {
        // child
        ngx_master_pid = getpid();
        setsid();
        umask(0);

        int fd = open("/dev/null", O_RDWR);
        dup2(fd, STDIN_FILENO);
        dup2(fd, STDOUT_FILENO);
        dup2(fd, STDERR_FILENO);

        if(fd > STDERR_FILENO) close(fd);


    }
    return 0;
}