#ifndef MEMDEBUG_H
#define MEMDEBUG_H

#include <stdio.h>
#include <stdlib.h>

#ifdef DEBUG_MEMORY
    #define malloc(size) dbg_malloc(size, __FILE__, __LINE__)
    #define calloc(n, size) dbg_calloc(n, size, __FILE__, __LINE__)
    #define realloc(ptr, size) dbg_realloc(ptr, size, __FILE__, __LINE__)
    #define free(ptr) dbg_free(ptr, __FILE__, __LINE__)

    void* dbg_malloc(size_t size, const char* file, int line) {
        void* ptr = malloc(size);
        printf("[MALLOC] %p (%zu bytes) at %s:%d\n", ptr, size, file, line);
        return ptr;
    }

    void* dbg_calloc(size_t n, size_t size, const char* file, int line) {
        void* ptr = calloc(n, size);
        printf("[CALLOC] %p (%zu bytes) at %s:%d\n", ptr, n*size, file, line);
        return ptr;
    }

    void* dbg_realloc(void* old, size_t size, const char* file, int line) {
        void* ptr = realloc(old, size);
        printf("[REALLOC] %p -> %p (%zu bytes) at %s:%d\n", old, ptr, size, file, line);
        return ptr;
    }

    void dbg_free(void* ptr, const char* file, int line) {
        printf("[FREE] %p at %s:%d\n", ptr, file, line);
        free(ptr);
    }
#endif

#endif

