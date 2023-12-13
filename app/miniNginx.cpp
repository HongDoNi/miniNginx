#include <iostream>
#include <signal.h>
#include <sys/signal.h>
#include <sys/wait.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>


void sig_usr(int signo) {
    printf("收到信号: %d, pid is %d\n", signo, getpid());
    switch(signo) {
        case(SIGCHLD):
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
            break;
    }
}


char **g_os_argv;
char *gp_envmem = nullptr;
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


    g_os_argv = (char**) argv;

    ngx_daemon();

    while(1) {
        sleep(1);
        printf("pid: %d sleep 1s\n", getpid());
    }

    printf("end\n");   
    return 0;

}
