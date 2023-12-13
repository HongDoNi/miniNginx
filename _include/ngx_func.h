//
// Created by 张懿 on 2023/12/8.
//

#ifndef MININGINX_NGX_FUNC_H
#define MININGINX_NGX_FUNC_H

#include <iostream>

class CConfig {
private:
    std::string mfile_name;

private:
    explicit CConfig(std::string);
    bool LoadConf(std::string);

public:
    static CConfig& getInstance();

};

#endif //MININGINX_NGX_FUNC_H
