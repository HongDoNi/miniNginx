#include <iostream>
#include <signal.h>
#include <unistd.h>

#include "ngx_func.h"
#include "ngx_marco.h"

typedef struct {
    int signo;
    const char *signame;
    void (*handler)(int, siginfo_t*, void*);
}ngx_signal_t;

static void ngx_signal_handler(int, siginfo_t*, void*);


ngx_signal_t signals[] = {
    {SIGHUP, "SIGHUP", ngx_signal_handler},
    {SIGINT, "SIGINT", ngx_signal_handler},
    {SIGTERM, "SIGTERM", ngx_signal_handler},
    {SIGCHLD, "SIGCHLD", ngx_signal_handler},
    {SIGQUIT, "SIGQUIT", ngx_signal_handler},
    {SIGIO, "SIGIO", ngx_signal_handler},
    {SIGSYS, "SIGSYS, SIG_INT", nullptr},
    {0, nullptr, nullptr}
};

int ngx_init_signals() {
    ngx_signal_t *sig;
    struct sigaction sa;
    
    for(sig = signals; sig -> signo != 0; ++ sig) {
        memset(&sa, 0, sizeof(sa));

        if(sig -> handler) {
            sa.sa_sigaction = sig -> handler;
            sa.sa_flags = SA_SIGINFO;
        }
        else {
            sa.sa_handler = SIG_IGN;
        }

        sigemptyset(&sa.sa_mask);

        if(sigaction(sig->signo, &sa, nullptr) == -1) {
            ngx_log_error_core(NGX_LOG_EMERG, errno, "sigaction(%s) failed", sig->signame);
        }

        else {
            ngx_log_stderr(0, "sigaction(%s) succed", sig->signame);
        }
        
    }
    return 0;

}




static void ngx_signal_handler(int signo, siginfo_t *siginfo, void* ucontext) {
   
    ngx_log_stderr(0,"收到信号: %d, pid is %d\n", signo, getpid());
    switch(signo) {
    case(SIGCHLD):{
        // pid_t pid = waitpid(-1, nullptr, WNOHANG);
        // if(pid == 0) {
        //     printf("waitipid returns 0\n");
        //     return;
        // }
        // else if(pid == -1) {
        //     printf("waitpid error\n");
        // }
        // else {
        //     printf("waitpid returns %d\n", pid);
        // }
    }
        break;
    case(SIGUSR1):{
        // printf("pid : %d  get signal : SIGUSR1 \n", getpid());
        // printf("sleep 10s \n");
        // sleep(10);
        
    }
        break;
    default: {
        // printf("get unknown signal\n");
    }
        break;
    
    }
}