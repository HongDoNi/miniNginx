#include <iostream>
#include <signal.h>
#include <sys/signal.h>
#include <unistd.h>


void sig_usr(int signo) {
    printf("收到信号: %d\n", signo);
}

char **g_os_argv;
char *gp_envmem = nullptr;


int main(const int argc, const char* const * argv) {
    g_os_argv = (char**) argv;

    sigset_t newmask, oldmask, pendmask;

    if(signal(SIGUSR1, sig_usr) == SIG_ERR) {
        printf("SIGUSR1 error\n");
    }

    if(signal(SIGUSR2, sig_usr) == SIG_ERR) {
        printf("SIGUSR2 error\n");
    }

    if(signal(SIGQUIT, sig_usr) == SIG_ERR) {
        printf("SIGQUIT error\n");
        exit(1);
    }

    sigemptyset(&newmask);
    // 把newmask信号集置零

    sigaddset(&newmask, SIGQUIT);
    // 把newmask信号集中的sigquit位置1，屏蔽该信号

    // sigprocmask()
    sigprocmask(SIG_BLOCK, &newmask, &oldmask);
    // SIG_BLOCK 意思是设置信号集，把当前信号集设为newmask，oldmask用于保存原有信号集


    while(1) {
        sleep(1);
        printf("sleep 1s\n");
    }

    printf("end\n");   
    return 0;

}
