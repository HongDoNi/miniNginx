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

    ngx_setproctitle("miniNgx: master ");

    // ngx_log_stderr(0,"worker counts: %d", process_count);

    ngx_start_worker_processes(process_count);

    sigemptyset(&set);
    sigprocmask(SIG_SETMASK, &set, nullptr);
    

    while(1) {
        sleep(5);
        ngx_log_error_core(0,0, "this MASTER process, pid is %d", getpid());
    }

   

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
        return;

    }
    else if(!pid) {
        // child process
        ngx_worker_process_cycle();
    }
    else {
        ngx_log_stderr(0, "Fork process FAIL");
        ngx_log_error_core(NGX_LOG_ALERT, errno, "ngx_spawn_process().fork() FAIL");
    }

}

void ngx_worker_process_cycle() {
    ngx_worker_process_init();

    ngx_setproctitle("miniNgx: worker");
    while(1) {
        ngx_log_error_core(0,0, "this WORKER process, pid is %d", getpid());

        sleep(1);
    }
    
}

void ngx_worker_process_init() {
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

    if(sigprocmask(SIG_SETMASK, &set, nullptr) == -1) {
        ngx_log_error_core(NGX_LOG_ALERT, errno, "ngx_worker_process_init()中sigpromask()失败");
    }


}