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

CConfig::~CConfig() {
    for(auto i : m_conf_item_list) {
        delete i;
    }
    m_conf_item_list.clear();
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

            if(buffer == "" || buffer[0] == '#') continue;
            else {
                int mid = -1, l = -1, r = -1;
                for(int i = 0; i < buffer.size(); ++ i) {
                    if(buffer[i] == '=') mid = i;
                    else if(buffer[i] == ' ') continue;
                    else if(mid == -1) { mid = 0; l = i; }
                    else if(mid != -1 && buffer[i] == ' ') r = i - 1;
                    else continue;
                }
                if(mid != -1 && mid != 0) {
                    std::string conf_n = buffer.substr(l, mid - l);
                    std::string conf_v = buffer.substr(mid + 1, r - mid -1);
                    printf("%s : %s \n", conf_n.c_str(), conf_v.c_str());
                    CConfItem* pc = new CConfItem{conf_n, conf_v};
                    this -> m_conf_item_list.push_back(pc);
                    
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


