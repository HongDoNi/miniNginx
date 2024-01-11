#pragma once

#include <atomic>
#include <pthread.h>
#include <vector>

class CThreadPool {
public:
    CThreadPool();
    ~CThreadPool();

public:
    bool create_threads(int thread_num);
    void stop_all_threads();
    void call_threads(int irmqc);

private:
    static void* thread_callback(void *thread_data);

private:
    class ThreadItem_ {
    public:
        pthread_t Handle;
        CThreadPool *p_this;
        bool if_running;

        ThreadItem_(CThreadPool* p):p_this(p), if_running(false) {}
        ~ThreadItem_() {}

    };

private:
    static pthread_mutex_t m_threadpool_mutex_;
    static pthread_cond_t m_threadpool_cond_;
    static bool m_shutdown_;

    int m_thread_num;

    std::atomic<int> m_running_thread_num;
    time_t m_last_emergence_time;

    std::vector<ThreadItem_*> m_threads;
    
};