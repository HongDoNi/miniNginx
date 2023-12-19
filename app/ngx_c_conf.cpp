#include "ngx_c_conf.h"

CConfig* CConfig::pm_conf{nullptr};
pthread_mutex_t CConfig::mutex; // 这句不可少，虽然和类中那句是一样的，但类中的作用是申明，这里是初始化。

CConfig* CConfig::GetInstance() {
    if(!pm_conf) {
        pthread_mutex_lock(&mutex);
        if(!pm_conf) {
            pm_conf = new CConfig();
        }
        pthread_mutex_unlock(&mutex);
    }
    return pm_conf;
}


CConfig::CConfig() {
    pthread_mutex_init(&mutex, NULL);
}

bool CConfig::LoadConf(std::string fn) {
    std::ifstream fp(fn);

    if(!fp.is_open()) {
        printf("open conf file error, check the filename\n");
        return false;
    }
    


    return true;
}

std::string CConfig::GetConfInfo(std::string cn) {
    for(auto i : m_conf_item_list) {
        if((*i).conf_name == cn) return (*i).conf_info;
    }
    return std::string{"Not Found"};
}


