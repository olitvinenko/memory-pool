#pragma once

#include "ObjectPool.hpp"
#include "Header.hpp"

#ifdef NDEBUG
#define _DBG_FILL_FREE
#else
#define _DBG_FILL_FREE(p, sz) memset(p, 0xfe, sz)
#endif

#define DECLARE_POOLED_ALLOCATION(cls)          \
private:                                        \
    static memory::ObjectPool<                  \
                  cls                           \
                , memory::details::Header       \
                                    > __pool;   \
    static void __fin(void *allocated)          \
    {                                           \
        __pool.Free(allocated);                 \
    }                                           \
public:                                         \
    void* operator new(size_t count)            \
    {                                           \
       using Header = memory::details::Header;  \
       assert(sizeof(cls) == count);            \
       void* _ptr = __pool.Alloc();             \
                                                \
       Header& hdr(*(Header*)_ptr);             \
       hdr.count = 0x80000000;                  \
       hdr.func = nullptr;                      \
                                                \
       return &hdr + 1;                         \
    }                                           \
                                                \
    void operator delete(void *p)               \
    {                                           \
        _DBG_FILL_FREE(p, sizeof(cls));         \
        using namespace memory::details;        \
        Header& hdr(ExtractFrom<void>(p));      \
        hdr.count &= 0x7fffffff;                \
        if (hdr.count == 0)                     \
            __pool.Free(&hdr);                  \
        else                                    \
            hdr.func = __fin;                   \
    }                                           \

#define IMPLEMENT_POOLED_ALLOCATION(cls)        \
    memory::ObjectPool<cls, memory::details::Header> cls::__pool;
