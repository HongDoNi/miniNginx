#pragma once

#include <iostream>

class CMemory
{
private:
     CMemory() {}

public:
    ~CMemory() {}

private:
    static CMemory* m_instance;
    static pthread_mutex_t mutex;

public:
    static CMemory* get_instance() {
        pthread_mutex_lock(&mutex);
        if(!m_instance) {
            m_instance = new CMemory();
            static CGarhuishou cl;
        }
        pthread_mutex_unlock(&mutex);
        return m_instance;
    }
    class CGarhuishou {
    public:
        ~CGarhuishou() {
            delete CMemory::m_instance;
            CMemory::m_instance = nullptr;
        }
    };
public:
    void* alloc_memory(int mem_count, bool if_memset);
    void free_memory(void* point);

};

