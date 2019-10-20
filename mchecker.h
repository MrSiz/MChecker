#ifndef __MCHECKER__
#define __MCHECKER__

#include <string>
#include <map>
#include <stdlib.h>

struct Info
{
    /* data */
    size_t line;
    size_t sz;
    const char *filename;
    Info(size_t l = -1, size_t s = 0, const char *f = "Null")
    : line(l), sz(s), filename(f) {}
};

#define malloc(size) chk_malloc(size, __FILE__, __LINE__)
#define free(ptr) chk_free(ptr, __FILE__, __LINE__)
#define calloc(nmemb, size) chk_calloc(nmemb, size, __FILE__, __LINE__)
#define realloc(ptr, size) chk_realloc(ptr, size, __FILE__, __LINE__)


extern std::map<void*, Info> mtable;
extern size_t total_bytes_alloc;
extern size_t total_bytes_del;


void *chk_malloc(size_t size, const char *filename, size_t line);
void *chk_calloc(size_t nmemb, size_t size, const char *filename, size_t line);
void *chk_realloc(void *ptr, size_t size, const char *filename, size_t line);

void chk_free(void *ptr, const char *filename, size_t line);

#ifdef __cplusplus

void *operator new(size_t size, const char *filename, size_t line);
void operator delete(void *ptr);

void *operator new[](size_t size, const char *filename, size_t line);
void operator delete[](void *ptr);

void deletep(const char *filename, size_t line);


#define new new(__FILE__, __LINE__)
#define delete deletep(__FILE__, __LINE__), delete

#endif


#endif
