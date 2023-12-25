#include <iostream>
#include <fstream>
#include <signal.h>
#include <sys/signal.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>

#include "ngx_c_conf.h"
#include "ngx_func.h"
#include "ngx_global.h"
#include "ngx_marco.h"

void sig_usr(int signo) {
    printf("收到信号: %d, pid is %d\n", signo, getpid());
    switch(signo) {
    case(SIGCHLD):{
        pid_t pid = waitpid(-1, nullptr, WNOHANG);
        if(pid == 0) {
            printf("waitipid returns 0\n");
            return;
        }
        else if(pid == -1) {
            printf("waitpid error\n");
        }
        else {
            printf("waitpid returns %d\n", pid);
        }
    }
        break;
    case(SIGUSR1):{
        printf("pid : %d  get signal : SIGUSR1 \n", getpid());
        printf("sleep 10s \n");
        sleep(10);
        
    }
        break;
    default: {
        printf("get unknown signal\n");
    }
        break;
    
    }
}


char** g_os_argv;
char *g_new_environ = nullptr;
size_t g_environ_len = 0;
pid_t ngx_pid;

int g_value = 0;


int ngx_daemon() {
    
    int fd;

    int pid = fork();
    if(pid < 0) {
        printf("fork fail\n");
        return 1;
    }
    else if(pid == 0) {
        printf("子进程, pid : %d \n", getpid());
    }
    else {
        printf("父进程, pid : %d \n", getpid());
        exit(0);
    }

    setsid();
    
    umask(0);

    fd = open("/dev/null", O_RDWR);

    if(dup2(fd, STDIN_FILENO) == -1) exit(1);
    if(dup2(fd, STDOUT_FILENO) == -1) exit(1);

    if(fd > STDERR_FILENO) {
        close(fd);
    }

    return 0;
}

int main(const int argc, const char* const * argv) {
    int exitcode = 0;

    ngx_pid = getpid();

    g_os_argv = (char**)argv;

    ngx_init_setproctitle();

    CConfig* pconf = CConfig::GetInstance();
    if(!pconf -> LoadConf("./nginx.conf")) {
        // printf("LoadConf error\n");
        ngx_log_stderr(0, "Config File loaded fail");
        exitcode = 2;
        goto lblexit;
    }

    ngx_log_init();

    if(ngx_init_signals() != 0) {
        ngx_log_stderr(0, "Init Signals fail");
        exitcode = 1;
        goto lblexit;
    }

    // while(1) {
        sleep(1);
        printf("pid: %d sleep 1s\n", getpid());
    // }

    

lblexit:
    if(g_new_environ) {
        delete [] g_new_environ;
        g_new_environ = nullptr;
    }


    printf("end\n");   
    return exitcode;

}
