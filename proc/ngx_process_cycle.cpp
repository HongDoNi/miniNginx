#include <iostream>
#include <errno.h>



#include "ngx_func.h"
#include "ngx_global.h"
#include "ngx_marco.h"
#include "ngx_c_conf.h"




void ngx_spawn_process();
void ngx_start_worker_processes(int);
void ngx_worker_process_init();
void ngx_worker_process_cycle();


void ngx_master_process_cycle() {
    sigset_t set;
    sigemptyset(&set);

    sigaddset(&set, SIGCHLD);
    sigaddset(&set, SIGALRM);
    sigaddset(&set, SIGIO);
    sigaddset(&set, SIGINT);
    sigaddset(&set, SIGHUP);
    sigaddset(&set, SIGUSR1);
    sigaddset(&set, SIGUSR2);
    sigaddset(&set, SIGWINCH);
    sigaddset(&set, SIGTERM);
    sigaddset(&set, SIGQUIT);

    if(sigprocmask(SIG_BLOCK, &set, nullptr) == -1) {
        ngx_log_error_core(NGX_LOG_ALERT, errno, "ngx_master_process_cycle()中sigpromask()失败");
    }




    CConfig* p_conf = CConfig::GetInstance();
    int process_count = stoi(p_conf -> GetConfInfo("WorkerProcesses"));

    ngx_setproctitle("nginx:  master");

    // ngx_start_worker_processes(process_count);

    sigemptyset(&set);

    while(1) {
        ngx_log_error_core(0,0, "this parent process, pid is %d", getpid());
    }

    // setsid();
    
    // umask(0);

    // int fd = open("/dev/null", O_RDWR);

    // if(dup2(fd, STDIN_FILENO) == -1) exit(1);
    // if(dup2(fd, STDOUT_FILENO) == -1) exit(1);

    // if(fd > STDERR_FILENO) {
    //     close(fd);
    // }

}

void ngx_start_worker_processes(int process_count) {
    while(process_count --) {
        ngx_spawn_process();
    }
}

void ngx_spawn_process() {
    pid_t pid = fork();
    if(pid > 0 ){
        // parent process
    }
    else if(!pid) {
        // child process
        ngx_worker_process_cycle();
    }
    else {
        ngx_log_stderr(0, "Fork process fail");
    }

}

void ngx_worker_process_cycle() {
    ngx_worker_process_init();

    ngx_setproctitle("miniNgx: worker");

    sleep(1);
}

void ngx_worker_process_init() {
    sigset_t set;
    sigprocmask(SIG_SETMASK, &set, nullptr);


}