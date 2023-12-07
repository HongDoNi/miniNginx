#include <iostream>
#include <signal.h>
#include <unistd.h>

void sig_usr(int signo) {
    if(signo == SIGUSR1) {
        printf("SIGUSR1信号..\n");
        sleep(5);
    }
    else if(signo == SIGUSR2) {
        printf("SIGUSR2信号..\n");
        sleep(5);
    }
}

char **g_os_argv;
char *gp_envmem = nullptr;


int main(const int argc, const char* const * argv) {
    g_os_argv = (char**) argv;
    for(int i = 0; )

    printf("end\n");
    return 0;
}
