#pragma once

#include "Header.hpp"
#include <cassert>

namespace memory
{
    using namespace details;

    template <class T>
    class ObjPtr
    {
        static size_t IncreaseCounter(T* ptr) { return ++ExtractFrom<T>(ptr).count; }
        static size_t DecreaseCounter(T* ptr) { return --ExtractFrom<T>(ptr).count; }
        
        static void TryToFinalize(T* ptr)
        {
            if (!ptr)
                return;
            
            Header& hdr(ExtractFrom<T>(ptr));
            hdr.count--;
            
            if (hdr.count == 0)
            {
                assert(hdr.func);
                hdr.func(&hdr);
            }
        }
        
    public:
        ObjPtr()
            : m_ptr(nullptr)
        { }
        
        ObjPtr(T* f)
            : m_ptr(f)
        {
            if (m_ptr) IncreaseCounter(m_ptr);
        }
        
        ObjPtr(const ObjPtr& f)
            : m_ptr(f.m_ptr)
        {
            if (m_ptr) IncreaseCounter(m_ptr);
        }

        ~ObjPtr()
        {
            TryToFinalize(m_ptr);
        }

        const ObjPtr& operator = (T* p)
        {
            if (p) IncreaseCounter(p);
            
            TryToFinalize(m_ptr);

            m_ptr = p;
            return *this;
        }

        operator T* () const
        {
            if (!m_ptr)
                return nullptr;

            Header& hdr(ExtractFrom<T>(m_ptr));
            size_t count = hdr.count & 0x80000000;
            if (count > 0)
                return m_ptr;

            return nullptr;
        }

        T* operator -> () const
        {
            assert(*this);
            return m_ptr;
        }
        
    private:
        T* m_ptr { nullptr };
    };

}// namespace memory

template<typename T>
using object_ptr = memory::ObjPtr<T>;

template<class U, class T>
inline U* ptr_dynamic_cast(T *src)
{
	assert(!src || object_ptr<T>(src));
	return dynamic_cast<U*>(src);
}

template<class U, class T>
inline U* ptr_dynamic_cast(const object_ptr<T> &src)
{
	return dynamic_cast<U*>(src.operator T*());
}

template<class U, class T>
inline U* ptr_static_cast(const object_ptr<T> &src)
{
	assert(!src || ptr_dynamic_cast<U>(src));
	return static_cast<U*>(src.operator T*());
}
