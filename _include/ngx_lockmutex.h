#pragma once

#include <pthread.h>

class CLock
{
private:
    pthread_mutex_t *mp_mutex_;

public:
    CLock(pthread_mutex_t *pm);
    ~CLock();
};

CLock::CLock(pthread_mutex_t *pm) {
    mp_mutex_ = pm;
    pthread_mutex_lock(mp_mutex_);
}

CLock::~CLock() {
    pthread_mutex_unlock(mp_mutex_);
}
