/*
Highly motivated on http://wyw.dcweb.cn/leakage.htm
*/
#ifndef _MEM_LEAK_DETECT
#define _MEM_LEAK_DETECT
#include "mem-leak-config.h"

#ifdef TURN_ON_MEM_LEAK_DETECTION
#include <cstdlib>
#include <cstdint>
#include <cstddef>

void* operator new(size_t size, const char* file, uint32_t line);
void* operator new[](size_t size, const char* file, uint32_t line);

void operator delete(void* ptr);
void operator delete[](void* ptr);

void* malloc(size_t size, const char* file, uint32_t line);
void* calloc(size_t num, size_t size, const char* file, uint32_t line);
void* realloc(void* ptr, size_t size, const char* file, uint32_t line);
void free(void* ptr, int /*just to be able to overload function*/);

#define DEBUG_NEW new(__FILE__, __LINE__)
#define new DEBUG_NEW

#define malloc(x) malloc((x), __FILE__, __LINE__)
#define free(x) free((x), 0)
#define calloc(x, y) calloc((x), (y), __FILE__, __LINE__)
#define realloc(x, y) realloc((x), (y), __FILE__, __LINE__)


#endif 
#endif