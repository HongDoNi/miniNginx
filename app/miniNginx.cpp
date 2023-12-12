#include <iostream>
#include <signal.h>
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
    sigaddset(&newmask, SIGQUIT);

    sigprocmask()


    while(1) {
        sleep(1);
        printf("sleep 1s\n");
    }

    printf("end\n");   
    return 0;

}
