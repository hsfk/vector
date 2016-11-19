#pragma once
#include <malloc.h>
#include <list>
#include <assert.h>

#define OUTTA_MEMORY throw "Out of memory."

template <typename _T, typename _U>
struct is_same
{
        static const bool value = false;
};

template <typename _T>
struct is_same<_T, _T>
{
        static const bool value = true;
};

struct first_fit_pool_tag { };
struct worst_fit_pool_tag { };
struct best_fit_pool_tag { };
//struct next_fit_pool_tag { };

template <typename _Ty, typename _Category = first_fit_pool_tag>
class Pool
{
public:
        typedef _Ty* pointer;
        typedef _Ty  value_type;
        typedef _Category pool_category;

        Pool();
        Pool(size_t _Size);
        ~Pool();

        pointer allocate(size_t _Size);
        void deallocate(pointer _Ptr, size_t _Count);

private:
        struct Chunk
        {
                Chunk(pointer _Begin, pointer _End)
                        : _Begin(_Begin), _End(_End) { }
                Chunk() : Chunk(nullptr, nullptr) { }
                inline size_t size() { return _End - _Begin; }

                pointer _Begin;
                pointer _End;
        };

        typedef typename::std::list<Chunk>           list;
        typedef typename::std::list<Chunk>::iterator iterator;

        pointer substract(iterator _It, size_t _Size);
        pointer first_fit(size_t _Size);
        pointer best_fit(size_t _Size);
        pointer worst_fit(size_t _Size);
        //pointer next_fit(size_t _Size);

        Chunk _Mem;
        list _Free;
};

template<typename _Ty, typename _Category>
Pool<_Ty, _Category>::Pool()
        : Pool(4000000)
{
}

template<typename _Ty, typename _Category>
Pool<_Ty, _Category>::Pool(size_t _Size)
{
        _Mem._Begin = (pointer)malloc(sizeof(value_type) * _Size);
        _Mem._End = _Mem._Begin + _Size;
        _Free.push_back(Chunk(_Mem._Begin, _Mem._End));
}

template<typename _Ty, typename _Category>
Pool<_Ty, _Category>::~Pool()
{
        free(_Mem._Begin);
}

template<typename _Ty, typename _Category>
inline typename::Pool<_Ty, _Category>::pointer Pool<_Ty, _Category>::allocate(size_t _Size)
{
        if (is_same<first_fit_pool_tag, _Category>::value)
                return first_fit(_Size);
        else if (is_same<worst_fit_pool_tag, _Category>::value)
                return worst_fit(_Size);
        else if (is_same<best_fit_pool_tag, _Category>::value)
                return best_fit(_Size);
        else
                assert(0); // unknown pool tag
        return pointer();
}

template<typename _Ty, typename _Category>
inline void Pool<_Ty, _Category>::deallocate(pointer _Ptr, size_t _Count)
{
        if (!_Ptr)
                return;
#if _DEBUG
        assert(_Ptr >= _Mem._Begin && (_Ptr + _Count) < _Mem._End);
#endif
        _Free.push_back(Chunk(_Ptr, _Ptr + _Count));
}

template<typename _Ty, typename _Category>
inline typename::Pool<_Ty, _Category>::pointer Pool<_Ty, _Category>::substract(iterator _It, size_t _Size)
{
        Chunk c = (*_It);
#if _DEBUG
        assert(c.size() >= _Size);
#endif
        if (_Size < c.size())
                _Free.insert(_It, Chunk(c._Begin + _Size, c._End));
        _Free.erase(_It);
        return c._Begin;
}

template<typename _Ty, typename _Category>
inline typename::Pool<_Ty, _Category>::pointer Pool<_Ty, _Category>::first_fit(size_t _Size)
{
        iterator _It = _Free.begin();
        while (_It != _Free.end())
        {
                Chunk c = (*_It);
                if (_Size <= c.size())
                        return substract(_It, _Size);
                _It++;
        }
        OUTTA_MEMORY;
        return pointer();
}

template<typename _Ty, typename _Category>
inline typename::Pool<_Ty, _Category>::pointer Pool<_Ty, _Category>::best_fit(size_t _Size)
{
        iterator _It = _Free.begin();
        iterator _Best = _Free.begin();
        while (_It != _Free.end())
        {
                Chunk c = (*_It);
                if (c.size() >= _Size && c.size() < (*_Best).size())
                        _Best = _It;
                _It++;
        }
        if ((*_Best).size() < _Size)
        {
                OUTTA_MEMORY;
                return pointer();
        }
        return substract(_Best, _Size);
}

template<typename _Ty, typename _Category>
inline typename::Pool<_Ty, _Category>::pointer Pool<_Ty, _Category>::worst_fit(size_t _Size)
{
        iterator _It = _Free.begin();
        iterator _Largest = _Free.begin();
        while (_It != _Free.end())
        {
                Chunk c = (*_It);
                if ((*_It).size() > (*_Largest).size())
                        _Largest = _It;
                _It++;
        }
        if ((*_Largest).size() < _Size)
        {
                OUTTA_MEMORY;
                return pointer();
        }
        return substract(_Largest, _Size);
}

//template<typename _Ty, typename _Category>
//inline Pool<_Ty, _Category>::pointer Pool<_Ty, _Category>::next_fit(size_t _Size)
//{
//        static iterator _Prev = _Free.begin();
//
//        iterator _It = _Prev;
//        while (_It != _Free.end())
//        {
//                Chunk c = (*_It);
//                if (_Size <= c.size())
//                {
//                        _Prev = _It;
//                        return substract(_It, _Size);
//                }
//                _It++;
//        }
//        _It = _Free.begin();
//        while (_It != _Free.end())
//        {
//                Chunk c = (*_It);
//                if (_Size <= c.size())
//                {
//                        _Prev = _It;
//                        return substract(_It, _Size);
//                }
//                _It++;
//        }
//        OUTTA_MEMORY;
//        return pointer();
//}
