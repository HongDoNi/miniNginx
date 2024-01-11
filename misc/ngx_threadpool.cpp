#include <unistd.h>
#include <pthread.h>

#include "ngx_func.h"
#include "ngx_memory.h"
#include "ngx_marco.h"
#include "ngx_global.h"


pthread_mutex_t CThreadPool::m_threadpool_mutex_ = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t CThreadPool::m_threadpool_cond_ = PTHREAD_COND_INITIALIZER;
bool CThreadPool::m_shutdown_ = false;


CThreadPool::CThreadPool() {
    m_running_thread_num = 0;
    m_last_emergence_time = 0;

}

bool CThreadPool::create_threads(int n) {
    ThreadItem_ *p_new_item;
    int err;
    
    for(int i = 0; i < n; ++ i) {
        p_new_item = new ThreadItem_(this);
        m_threads.push_back(p_new_item);   
        err = pthread_create(&p_new_item -> Handle, nullptr, thread_callback, p_new_item);
        if(err != 0) {
            ngx_log_error_core(NGX_LOG_ERR, err, "CThreadPool::call_threads pthread_create FAIL");
            return false;
        }
        else {

        }
    }
lblfor:
    for(auto iter = m_threads.begin(); iter != m_threads.end(); ++ iter) {
        if( (*iter) -> if_running == false) {
            usleep(100* 1000);
            goto lblfor;
        }
    }

    return true;
}

void* CThreadPool::thread_callback(void* thread_data) {
    ThreadItem_* p_thread_item = (ThreadItem_*)thread_data;
    CThreadPool* p_thread_pool = p_thread_item -> p_this;

    char* recv_buf = nullptr;
    CMemory* p_mem = CMemory::get_instance();
    pthread_t thread_id = pthread_self();
    int err;

    while(true) {
        err = pthread_mutex_lock(&m_threadpool_mutex_);
        if(err != 0) ngx_log_error_core(NGX_LOG_ERR, err, "CThreadPool::thread_callback pthread_mutex_lock FAIL");

        while(m_shutdown_ == false && ((recv_buf = g_socket.out_from_msg_recv_queue()) == nullptr)) {
            // 处理被唤醒但是没有消息可以处理（消息被其他唤醒的现场处理掉了）的线程

            if(p_thread_item -> if_running == false) p_thread_item -> if_running = true;

            pthread_cond_wait(&m_threadpool_cond_, &m_threadpool_mutex_);
        }

        err = pthread_mutex_unlock(&m_threadpool_mutex_);
        if(err != 0) ngx_log_error_core(NGX_LOG_ERR, err, "CThreadPool::thread_callback pthread_mutex_unlock FAIL");

        if(m_shutdown_) {
            if(recv_buf != nullptr) {
                p_mem -> free_memory(recv_buf);
            }
            break;
        }

        ++ (p_thread_pool -> m_running_thread_num);

        ngx_log_stderr(0,"执行开始---begin,tid=%ui!",thread_id);
        sleep(5); //临时测试代码
        ngx_log_stderr(0,"执行结束---end,tid=%ui!",thread_id);
        p_mem -> free_memory(recv_buf);

        -- p_thread_pool -> m_running_thread_num;

    }
    return (void*)0;

}

void CThreadPool::call_threads(int n) {
    int err = pthread_cond_signal(&m_threadpool_cond_);
    if(err != 0) {
        ngx_log_error_core(NGX_LOG_ERR, err, "CThreadPool::call_thread pthread_cond_signal FAIL");
    }

    if(m_thread_num == m_running_thread_num) {
        time_t current_time = time(nullptr);
        if(current_time - m_last_emergence_time > 10) {
            m_last_emergence_time = current_time;
            ngx_log_error_core(NGX_LOG_ALERT, 0, "There is No free thread!");
        }
    }

    return;
    
}

void CThreadPool::stop_all_threads() {
    if(m_shutdown_) return;
    m_shutdown_ = true;

    int err = pthread_cond_broadcast(&m_threadpool_cond_);
    if(err != 0) {
        ngx_log_error_core(NGX_LOG_ERR, err, "CThreadPool::stop_all_threads pthread_cond_broadcast FAIL");
        return;
    }

    for(auto iter = m_threads.begin(); iter != m_threads.end(); ++ iter) {
        err = pthread_join((*iter) -> Handle, nullptr);
        if(err != 0) {
            ngx_log_error_core(NGX_LOG_ERR, err, "CThreadPool::stop_all_threads pthread_join FAIL");
            return ;
        }
    }

    pthread_mutex_destroy(&m_threadpool_mutex_);
    pthread_cond_destroy(&m_threadpool_cond_);

    for(auto iter = m_threads.begin(); iter != m_threads.end(); ++ iter) {
        if(*iter) delete *iter;
    }

    m_threads.clear();

    ngx_log_error_core(NGX_LOG_INFO, 0,"All threads stopped");
    return;
    
}

CThreadPool::~CThreadPool() {

}