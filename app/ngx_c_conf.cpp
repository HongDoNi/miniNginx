#include "ngx_c_conf.h"

CConfig* CConfig::pmconf{nullptr};
pthread_mutex_t CConfig::mutex; // 这句不可少，虽然和类中那句是一样的，但类中的作用是申明，这里是初始化。

CConfig* CConfig::getInstance() {
    if(!pmconf) {
        pthread_mutex_lock(&mutex);
        if(!pmconf) {
            pmconf = new CConfig();
        }
        pthread_mutex_unlock(&mutex);
    }
    return pmconf;
}


CConfig::CConfig() {
    pthread_mutex_init(&mutex, NULL);
}

bool CConfig::LoadConf(std::string fn) {
    if(!fopen(fn.c_str(), "r")) {
        printf("open conf file error, check the filename\n");
        return false;
    }
    return true;
}
