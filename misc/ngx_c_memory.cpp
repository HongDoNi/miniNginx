#include <cstring>
#include "ngx_memory.h"

CMemory* CMemory::m_instance = nullptr;
 pthread_mutex_t CMemory::mutex;

void* CMemory::alloc_memory(int mem_count, bool if_memset) {
    char* p = new char[mem_count];
    if(if_memset) {
        memset(p, 0, mem_count);
    }
    return (void*)p;
}

void CMemory::free_memory(void *point) {
    delete [] ((char*)point);
}