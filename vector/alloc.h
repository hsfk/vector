#pragma once

#include <cstdint>
#include "pool.h"

template <typename _Ty>
struct _TypeWrapper
{
        typedef _Ty        value_type;
        typedef _Ty*       pointer;
        typedef _Ty&       reference;
        typedef const _Ty* const_pointer;
        typedef const _Ty& const_reference;
        typedef size_t     size_type;
        typedef ptrdiff_t  difference_type;
};

template <typename _Ty, typename _PoolCat = first_fit_pool_tag>
struct Allocator : public _TypeWrapper<_Ty>
{
        Allocator();
        ~Allocator();
        Allocator(const Allocator& _Alloc);
        Allocator& operator=(const Allocator& _Right);

        pointer allocate(size_type _Count);
        void deallocate(pointer _Ptr, size_t _Count);
        void destroy(pointer _Ptr);
        template <typename _T, typename... _ArgsTy>
        void construct(_T* _Ptr, _ArgsTy&&... _Args);
private:
        typedef Pool<value_type, _PoolCat> pool;

        pool* _Pool;
        size_type* _Pool_refs;
};

template<typename _Ty, typename _PoolCat>
template<typename _T, typename ..._ArgsTy>
inline void Allocator<_Ty, _PoolCat>::construct(_T* _Ptr, _ArgsTy&& ..._Args)
{
        new ((void*)_Ptr) _T(std::forward<_ArgsTy>(_Args)...);
}

template<typename _Ty, typename _PoolCat>
Allocator<_Ty, _PoolCat>::Allocator()
{
        _Pool = new pool;
        _Pool_refs = new size_type;
        *_Pool_refs = 1;
}

template<typename _Ty, typename _PoolCat>
Allocator<_Ty, _PoolCat>::~Allocator()
{
        *_Pool_refs -= 1;
        if (*_Pool_refs == 0)
        {
                delete _Pool;
                delete _Pool_refs;
        }
}

template<typename _Ty, typename _PoolCat>
Allocator<_Ty, _PoolCat>::Allocator(const Allocator& _Alloc)
{
        *this = _Alloc;
}

template<typename _Ty, typename _PoolCat>
inline Allocator<_Ty, _PoolCat>& Allocator<_Ty, _PoolCat>::operator=(const Allocator& _Right)
{
        _Pool = _Right._Pool;
        _Pool_refs = _Right._Pool_refs;
        *_Pool_refs += 1;
        return *this;
}

template<typename _Ty, typename _PoolCat>
inline typename::Allocator<_Ty, _PoolCat>::pointer Allocator<_Ty, _PoolCat>::allocate(size_type _Count)
{
        return _Pool->allocate(_Count);
}

template<typename _Ty, typename _PoolCat>
inline void Allocator<_Ty, _PoolCat>::deallocate(pointer _Ptr, size_t _Count)
{
        return _Pool->deallocate(_Ptr, _Count);
}

template<typename _Ty, typename _PoolCat>
inline void Allocator<_Ty, _PoolCat>::destroy(pointer _Ptr)
{
        _Ptr->~value_type();
}