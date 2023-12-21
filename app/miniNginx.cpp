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


    g_os_argv = (char**)argv;

    // ngx_daemon();

    // if(signal(SIGUSR1, sig_usr) == SIG_ERR) {
    //     printf("signal error\n");
    // }
    
    // printf("before:\n");
    // printf("environ stard address: %p\n", environ[0]);
    // printf("environ[0]: %s\n", environ[0]);
    // printf("environ[1]: %s\n", environ[1]);
    // printf("environ[2]: %s\n", environ[2]);
    // printf("environ[3]: %s\n", environ[3]);
    

    ngx_init_setproctitle();

    // printf("after:\n");
    // printf("environ stard address: %p\n", environ[0]);
    // printf("environ[0]: %s\n", environ[0]);
    // printf("environ[1]: %s\n", environ[1]);
    // printf("environ[2]: %s\n", environ[2]);
    // printf("environ[3]: %s\n", environ[3]);

    CConfig* pconf = CConfig::GetInstance();
    if(!pconf -> LoadConf("./nginx.conf")) {
        printf("LoadConf error\n");
        exit(1);
    }

    // printf("before:\n");
    // printf("argv[0]:%s\n", argv[0]);
    // printf("strlen argv[0]: %zu\n", strlen(argv[0]));
    // ngx_setproctitle("nginx: master");
    // printf("after:\n");
    // printf("argv[0]:%s\n", argv[0]);
    // printf("strlen argv[0]: %zu\n", strlen(argv[0]));

    







    // while(1) {
        sleep(1);
        printf("pid: %d sleep 1s\n", getpid());
    // }

    if(g_new_environ) {
        delete [] g_new_environ;
        g_new_environ = nullptr;
    }

    printf("end\n");   
    return 0;

}
