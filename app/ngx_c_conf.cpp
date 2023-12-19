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
    std::ifstream fp;
    fp.open(fn, std::ios::in);

    if(!fp.is_open()) {
        printf("open conf file error, check the filename\n");
        return false;
    }
    else {
        std::string buffer;
        while(getline(fp, buffer)) {
            std::cout << buffer << "e" << std::endl;
            if(buffer == "") std::cout << "empty line" << std::endl;
            else {
                auto l = buffer.end();
                auto r = buffer.end();
                auto temp = buffer.begin();

                for(auto it = buffer.begin(); it != buffer.end(); ++ it) {
                    std::string conf_name{""};
                    std::string conf_value("");

                    if(*it == ' ') {
                        if(l != buffer.end()) {
                            if(conf_name != "") {
                                conf_value = buffer.substr(l, r)
                            }
                        }
                        else continue;
                    }
                    else if(*it == '#') break;
                    else {
                        if(l == buffer.end()) l = it;
                        else r = it;                        

                    }
                }
            }
        }
            
            
    }

    return true;
}

std::string CConfig::GetConfInfo(std::string cn) {
    for(auto i : m_conf_item_list) {
        if((*i).conf_name == cn) return (*i).conf_info;
    }
    return std::string{"Not Found"};
}


