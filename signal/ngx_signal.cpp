#include <iostream>
#include <signal.h>
#include <unistd.h>
#include <sys/wait.h>

#include "ngx_func.h"
#include "ngx_marco.h"
#include "ngx_global.h"



static void ngx_signal_handler(int, siginfo_t*, void*);
static void ngx_process_get_status();

typedef struct {
    int signo;
    const char *signame;
    void (*handler)(int, siginfo_t*, void*);
}ngx_signal_t;

ngx_signal_t signals[] = {
    {SIGHUP, "SIGHUP", ngx_signal_handler},
    {SIGINT, "SIGINT", ngx_signal_handler},
    {SIGTERM, "SIGTERM", ngx_signal_handler},
    {SIGCHLD, "SIGCHLD", ngx_signal_handler},
    {SIGQUIT, "SIGQUIT", ngx_signal_handler},
    {SIGIO, "SIGIO", ngx_signal_handler},
    {SIGSYS, "SIGSYS, SIG_INT", nullptr},
    {SIGUSR1, "SIGUSR1", ngx_signal_handler},
    {SIGUSR2, "SIGUSR2", ngx_signal_handler},
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
   
    // ngx_log_stderr(0,"收到信号: %d, pid is %d\n", signo, getpid());
    ngx_log_error_core(0,0,"收到信号: %d, pid is %d\n", signo, getpid());
    // sleep(5);
    char *action;
    if(ngx_process == NGX_MASTER_PROCESS) {
        switch(signo) {
        case SIGCHLD :
            int status;
            ngx_reap = 1;
            // waitpid(-1, &status, WNOHANG);
            break;
        case SIGUSR1:
            break;
        case SIGUSR2:
            break;
        default:
            break;

        }
    }
    else if (ngx_process == NGX_WORKER_PROCESS) {
        // worker 进程信号处理函数
    }   
    else {
        // 基本不会到这分支
    }

    
    ngx_signal_t* p_sig = signals;
    for(;p_sig; ++ p_sig) {
        if(p_sig -> signo == signo) {
            break;
        }

    }
    action = (char *)"";

    if(siginfo && siginfo -> si_pid) {  

        ngx_log_error_core(NGX_LOG_NOTICE,0,"signal %d (%s) received from %P%s", signo, p_sig->signame, siginfo->si_pid, action); 
    }
    else {
        ngx_log_error_core(NGX_LOG_NOTICE,0,"signal %d (%s) received %s",signo, p_sig->signame, action);//没有发送该信号的进程id，所以不显示发送该信号的进程id
    }

    if(signo == SIGCHLD) {
        // 测试下给子进程发送SIGCHLD会执行该函数吧?
        ngx_process_get_status();
    }
    return;
    
}


static void ngx_process_get_status() {
    pid_t pid;
    int status;
    int err;
    int one = 0;


    while(1) {
        pid = waitpid(-1, &status, WNOHANG);
        
        if(pid == 0) {
            // do nothing
        }
        else if(pid == -1) {
            err = errno;
            if(err == EINTR) {
                continue;
            }

            if(err == ECHILD) {
                ngx_log_error_core(NGX_LOG_INFO, err, "waitpid() FAIL");
                return;
            }
            ngx_log_error_core(NGX_LOG_ALERT, err, "waitpid() FAIL");
            return;
        }

        one = 1;
        if(WTERMSIG(status)) {
            ngx_log_error_core(NGX_LOG_ALERT, 0, "pid = %P exited on signal %d", pid, WTERMSIG(status));
        }
        else {
            ngx_log_error_core(NGX_LOG_NOTICE, 0, "pid = %P, exited with code %d", pid, WEXITSTATUS(status));
        }
    }
}