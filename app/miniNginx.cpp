#include <iostream>
#include <signal.h>
#include <sys/signal.h>
#include <sys/wait.h>
#include <unistd.h>


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

int main(const int argc, const char* const * argv) {
    g_os_argv = (char**) argv;

    pid_t pid;

    sigset_t newmask, oldmask, pendmask;

    if(signal(SIGUSR1, sig_usr) == SIG_ERR) {
        printf("SIGUSR1 error\n");
    }

    if(signal(SIGUSR2, sig_usr) == SIG_ERR) {
        printf("SIGUSR2 error\n");
    }

    if(signal(SIGQUIT, sig_usr) == SIG_ERR) {
        printf("SIGQUIT error\n");
    }
    

    sigemptyset(&newmask);
    // 把newmask信号集置零

    sigaddset(&newmask, SIGQUIT);
    // 把newmask信号集中的sigquit位置1，屏蔽该信号

    // sigprocmask()
    sigprocmask(SIG_BLOCK, &newmask, &oldmask);
    // SIG_BLOCK 意思是设置信号集，把当前信号集设为newmask，oldmask用于保存原有信号集

    pid = fork();
    
    if(pid < 0) {
        printf("创建子进程失败\n");
        exit(1);
    }
    else if(pid == 0) {
        while(1) {
            g_value += 10;
            sleep(1);
            printf("子进程, pid: %d, g_value: %d\n", getpid(), g_value);
        }
        
        
    }
    else {
        while(1) {
            g_value += 1;
            sleep(1);
            printf("父进程, pid: %d, g_value: %d\n", getpid(), g_value);
        }
        
    }

    if(signal(SIGCHLD, sig_usr) == SIG_ERR) {
        printf("SIGCHLD error\n");
    }

    while(1) {
        sleep(1);
        printf("pid: %d sleep 1s\n", getpid());
    }

    printf("end\n");   
    return 0;

}
