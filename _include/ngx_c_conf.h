//
// Created by 张懿 on 2023/12/8.
//

#ifndef MININGINX_NGX_C_CONF_H
#define MININGINX_NGX_C_CONF_H

#include <iostream>
#include <vector>
#include <unistd.h>
#include <fstream>

typedef struct {
    std::string conf_name;
    std::string conf_info;
}CConfItem;

class CConfig {
private:
    std::string m_file_name;
    
private:
    static CConfig* pm_conf;

private:
    CConfig();
    CConfig(const CConfig& other) = delete;
    CConfig& operator=(const CConfig& other) = delete;

public: 
    ~CConfig(); // 单例类中的析构函数不要做delete pmconf的动作，会引起循环释放统一内存地址
    
    static CConfig* GetInstance();
    static pthread_mutex_t mutex;
    class Garbo {
    public:
        Garbo() { printf("Garbo()\n"); }
        ~Garbo() {
            if(pm_conf) delete pm_conf;
            pm_conf = nullptr;
        }
    };
    static Garbo garbo; // 这个garbo难道不需要在哪里初始化吗？是因为被自动初始化吗？

public:
    bool LoadConf(std::string);
    std::string GetConfInfo(std::string);
    std::vector<CConfItem*> m_conf_item_list;

};



#endif //MININGINX_NGX_C_CONF_H
