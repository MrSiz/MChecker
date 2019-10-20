#include <iostream>
#include <string.h>


#include "mchecker.h"



#undef malloc
#undef free
#undef calloc
#undef realloc


std::map<void*, Info> mtable;
size_t total_bytes_alloc, total_bytes_del;

#ifdef __cplusplus
#undef new
#undef delete
#endif

static bool flag = false;


static void get_statistic() {
    std::cout << "\n-----SUMMARY-----\n";
    std::cout << "Total memory allocated: " << total_bytes_alloc << " bytes\n";
    std::cout << "Total memory freed: " << total_bytes_del << " bytes\n";
    std::cout << "Tatal memory leaked: " << total_bytes_alloc - total_bytes_del << " bytes\n";
    for (std::map<void*, Info>::iterator it = mtable.begin(); it != mtable.end(); ++it) {
        std::cout << "Warning: " << (it->second).sz << " bytes at " << (it->second).filename << " line "
                  << (it->second).line << " were not freed!!\n";
    }
    return ;
}



void *chk_malloc(size_t size, const char *filename, size_t line) {
    std::cout << "memory allocated at file " << filename << " line: " << line 
              << " total: " << size << " bytes" << std::endl;
    void *ptr = malloc(size);
    if (ptr == NULL) {
        std::cerr << "fail to allocate memory at file " << filename << " line: " << line << "\n";
        exit(1);
    }
    
    if (!flag) {
        atexit(get_statistic);
        flag = true;
    }

    total_bytes_alloc += size;
    mtable[(void*)ptr] = Info(line, size, filename);
    return ptr;
}

void *chk_calloc(size_t nmemb, size_t size, const char *filename, size_t line) {
    std::cout << "memory allocated at file " << filename << " line: " << line 
        << " total: " << size * nmemb << " bytes" << std::endl;
    void *ptr = calloc(nmemb, size);
    if (ptr == NULL) {
        return NULL;
    }

    if (!flag) {
        atexit(get_statistic);
        flag = true;
    }

    total_bytes_alloc += size * nmemb;
    mtable[(void*)ptr] = Info(line, size * nmemb, filename);
    return ptr;
}

void *chk_realloc(void *ptr, size_t size, const char *filename, size_t line) {
    std::cout << "memory allocated at file " << filename << " line: " << line 
    << " total: " << size << " bytes" << std::endl;
    
    void *new_ptr = malloc(size);
    if (new_ptr == NULL) {
        std::cerr << "fail to allocate memory at file " << filename << " line: " << line << "\n";
        exit(1);
    }
    mtable[(void*)new_ptr] = Info(line, size, filename);
    total_bytes_alloc += size;
    if (ptr) {
        memcpy(new_ptr, ptr, mtable[(void*)ptr].sz);
        size_t old_sz = mtable[(void*)ptr].sz;
        std::cout << "memory freed in \"realloc function\": " << old_sz << " bytes\n";
        free(ptr);
        mtable.erase((void*)ptr);
        total_bytes_del += old_sz; 
    }
    return new_ptr;
}


void chk_free(void *ptr, const char *filename, size_t line) {
    std::cout << "memory freed at file "<< filename << " line: " << line;
    std::map<void*, Info>::iterator it = mtable.find((void*)ptr);
    if (it != mtable.end()) {
        free(ptr);   
        size_t temp = mtable[(void*)ptr].sz;
        mtable.erase(it);
        total_bytes_del += temp;
        std::cout << " total: " << temp << " bytes\n";
    }
    return ;
}

#ifdef __cplusplus

void deletep(const char *filename, size_t line) {
    std::cout << "memory freed at file "<< filename << " line: " << line;
    return ;
}


void *operator new(size_t size, const char *filename, size_t line) {
    void *ptr = malloc(size);
    if (ptr == NULL) {
        std::cerr << "fail to allocate memory at file " << filename << " line: " << line << "\n";
        exit(0);
    }
    std::cout << "memory allocated at file " << filename << " line: " << line 
    << " total: " << size << " bytes" << std::endl;

    total_bytes_alloc += size;
    mtable[(void*)ptr] = Info(line, size, filename);
    return ptr;
}  

void operator delete(void *ptr) throw () {
    if (mtable.count((void*)ptr)) {
        free(ptr);
        size_t temp = mtable[(void*)ptr].sz;
        total_bytes_del += temp;
        std::cout << " total: " << temp << " bytes" << std::endl;
        mtable.erase((void*)ptr);
    }
    return ;
}

void *operator new[](size_t size, const char *filename, size_t line) {
    return operator new(size, filename, line);
} 


void operator delete[](void *ptr) throw() {
    operator delete(ptr);
    return ;
}

#endif

