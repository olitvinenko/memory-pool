#pragma once

#include <stddef.h>

namespace memory
{
    namespace details
    {
        struct Header
        {
            using FinalizeFunc = void (*)(void*);

            FinalizeFunc func;
            size_t count;
        };

        template<typename T>
        Header& ExtractFrom(T* p)
        {
            return ((Header*)p)[-1];
        }
    } // namespace details
} // namespace memory
