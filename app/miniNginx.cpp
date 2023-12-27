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
#include "ngx_marco.h"

char** g_os_argv;
char *g_new_environ = nullptr;
size_t g_environ_len = 0;
bool g_enable_daemon = false;
pid_t ngx_master_pid;
int ngx_process;
sig_atomic_t ngx_reap;

int g_value = 0;

void free_source();

int ngx_daemon() {
    ngx_process = NGX_MASTER_PROCESS;
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
    int exitcode = 0;
    ngx_reap = 0;
    ngx_master_pid = getpid();

    g_os_argv = (char**)argv;

    ngx_log.fd = -1;

    ngx_init_setproctitle();
    // ngx_setproctitle("miniNgx: master");

    CConfig* pconf = CConfig::GetInstance();
    if(!pconf -> LoadConf("./nginx.conf")) {
        // printf("LoadConf error\n");
        ngx_log_stderr(0, "Config File loaded fail");
        exitcode = 2;
        goto lblexit;
    }

    ngx_log_init();

    if(ngx_init_signals() != 0) {
        ngx_log_stderr(0, "Init Signals fail");
        exitcode = 1;
        goto lblexit;
    }

    if(stoi(pconf -> GetConfInfo("Daemon")) == 1) {
        int res_daemon = ngx_enable_daemon();
        if(res_daemon == -1) {
            exitcode = 1;
            goto lblexit;
        }
        else if(res_daemon == 1) {
            free_source();
            exitcode = 0;
            return exitcode;
        }
        g_enable_daemon = true;
        
    }
    

    ngx_master_process_cycle();

    while(1) {
        sleep(1);
        printf("pid: %d sleep 1s\n", getpid());
    }

    

lblexit:
    if(g_new_environ) {
        delete [] g_new_environ;
        g_new_environ = nullptr;
    }


    printf("end\n");   
    return exitcode;

}

void free_source() {
    if(g_new_environ) {
        delete [] g_new_environ;
        g_new_environ = nullptr;
    }
    if(ngx_log.fd != STDERR_FILENO && ngx_log.fd != -1){
        close(ngx_log.fd);
        ngx_log.fd = -1;
    }
}
