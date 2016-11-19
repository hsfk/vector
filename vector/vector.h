#pragma once

#include "alloc.h"
#define _Assert(expr) if (!(expr)) throw 122

struct input_iterator_tag { };
struct output_iterator_tag { };
struct forward_iterator_tag { };
struct bidirectional_iterator_tag { };
struct random_access_iterator_tag { };

template <typename _Ty, typename _Category>
struct _IteratorBase : _TypeWrapper<_Ty>
{
        typedef _Category iterator_category;
};

template <typename _VecTy, typename _Category>
struct _VectorIterator : _IteratorBase<typename _VecTy::value_type, _Category>
{
        _VectorIterator();
        _VectorIterator(pointer _Ptr);
        _VectorIterator(const _VectorIterator& _It);
        _VectorIterator& operator=(const _VectorIterator& _Right);
       
        difference_type operator-(const _VectorIterator& _Right);
        difference_type operator+(const _VectorIterator& _Right);

        _VectorIterator operator+(difference_type _Count);
        _VectorIterator operator-(difference_type _Count);

        _VectorIterator& operator++();
        _VectorIterator& operator--();
        _VectorIterator operator++(int);
        _VectorIterator operator--(int);

        bool operator<(const _VectorIterator& _Right);
        bool operator!=(const _VectorIterator& _Right);
        bool operator==(const _VectorIterator& _Right);

        reference operator*();
        pointer operator->();

        pointer _Ptr;

#if _DEBUG
        bool in_range(const pointer _Ptr)
        {
                pointer _Begin = _Vector->data();
                pointer _End = _Begin + _Vector->size();
                return (_Ptr >= _Begin && _Ptr < _End);
        }

        _VecTy* _Vector;
#endif
};

template<typename _VecTy, typename _Category>
_VectorIterator<_VecTy, _Category>::_VectorIterator()
        : _Ptr()
{
#if _DEBUG
        _Vector = nullptr;
#endif
}

template<typename _VecTy, typename _Category>
inline _VectorIterator<_VecTy, _Category>::_VectorIterator(pointer _Ptr)
{
        this->_Ptr = _Ptr;
}

template<typename _VecTy, typename _Category>
_VectorIterator<_VecTy, _Category>::_VectorIterator(const _VectorIterator& _It)
        : _VectorIterator(_It._Ptr)
{
#if _DEBUG
        _Vector = _It._Vector;
#endif
}

template<typename _VecTy, typename _Category>
inline _VectorIterator<_VecTy, _Category>& _VectorIterator<_VecTy, _Category>::operator=(const _VectorIterator& _Right)
{
        _Ptr = _Right._Ptr;
#if _DEBUG
        _Vector = _Right._Vector;
#endif
        return *this;
}

template<typename _VecTy, typename _Category>
inline typename::_VectorIterator<_VecTy, _Category>::difference_type _VectorIterator<_VecTy, _Category>::operator-(const _VectorIterator& _Right)
{
        return _Ptr - _Right._Ptr;
}

template<typename _VecTy, typename _Category>
inline typename::_VectorIterator<_VecTy, _Category>::difference_type _VectorIterator<_VecTy, _Category>::operator+(const _VectorIterator& _Right)
{
        return _Ptr + _Right._Ptr;
}

template<typename _VecTy, typename _Category>
inline _VectorIterator<_VecTy, _Category> _VectorIterator<_VecTy, _Category>::operator+(difference_type _Count)
{
        return _VectorIterator(_Ptr + _Count);
}

template<typename _VecTy, typename _Category>
inline _VectorIterator<_VecTy, _Category> _VectorIterator<_VecTy, _Category>::operator-(difference_type _Count)
{
        return _VectorIterator(_Ptr - _Count);
}

template<typename _VecTy, typename _Category>
inline _VectorIterator<_VecTy, _Category>& _VectorIterator<_VecTy, _Category>::operator++()
{
        _Ptr++;
        return *this;
}

template<typename _VecTy, typename _Category>
inline _VectorIterator<_VecTy, _Category>& _VectorIterator<_VecTy, _Category>::operator--()
{
        _Ptr--;
        return *this;
}

template<typename _VecTy, typename _Category>
inline _VectorIterator<_VecTy, _Category> _VectorIterator<_VecTy, _Category>::operator++(int)
{
        _VectorIterator it = *this;
        ++*this;
        return it;
}

template<typename _VecTy, typename _Category>
inline _VectorIterator<_VecTy, _Category> _VectorIterator<_VecTy, _Category>::operator--(int)
{
        _VectorIterator it = *this;
        --*this;
        return it;
}

template<typename _VecTy, typename _Category>
inline bool _VectorIterator<_VecTy, _Category>::operator<(const _VectorIterator& _Right)
{
        return _Ptr < _Right._Ptr;;
}

template<typename _VecTy, typename _Category>
inline bool _VectorIterator<_VecTy, _Category>::operator!=(const _VectorIterator& _Right)
{
        return _Ptr != _Right._Ptr;
}

template<typename _VecTy, typename _Category>
inline bool _VectorIterator<_VecTy, _Category>::operator==(const _VectorIterator& _Right)
{
        return _Ptr == _Right._Ptr;
}

template<typename _VecTy, typename _Category>
inline typename::_VectorIterator<_VecTy, _Category>::reference _VectorIterator<_VecTy, _Category>::operator*()
{
#if _DEBUG
        _Assert(in_range(_Ptr));
#endif
        return *_Ptr;
}

template<typename _VecTy, typename _Category>
inline typename::_VectorIterator<_VecTy, _Category>::pointer _VectorIterator<_VecTy, _Category>::operator->()
{
#if _DEBUG
        _Assert(in_range(_Ptr));
#endif
        return _Ptr;
}

template <typename _VecTypes>
class _VectorVal
{
public:
        typedef typename _VecTypes::pointer pointer;

        _VectorVal()
        {
                _First = pointer();
                _Last = pointer();
                _End = pointer();
        }

        _VectorVal& operator=(const _VectorVal& _Right) noexcept
        {
                _First = _Right._First;
                _Last = _Right._Last;
                _End = _Right._End;
        }

        pointer _First;
        pointer _Last;
        pointer _End;
};

template <typename _AllocTy>
class _VectorBase : public _TypeWrapper<typename _AllocTy::value_type>
{
public:
        typedef _TypeWrapper<typename _AllocTy::value_type> _MyTypes;
        typedef _VectorVal<_MyTypes> _MyVal;
        typedef _AllocTy allocator_type;

        explicit _VectorBase(const allocator_type& _Alloc = _AllocTy());

        allocator_type get_allocator() const noexcept;
        value_type* data();
        value_type* data() const;
protected:
        _MyVal _Data;
private:
        allocator_type _Allocator;
};

template<typename _AllocTy>
_VectorBase<_AllocTy>::_VectorBase(const allocator_type& _Alloc)
        : _Allocator(_Alloc)
{
}

template<typename _AllocTy>
inline typename::_VectorBase<_AllocTy>::allocator_type _VectorBase<_AllocTy>::get_allocator() const noexcept
{
        return _Allocator;
}

template<typename _AllocTy>
inline typename::_VectorBase<_AllocTy>::value_type* _VectorBase<_AllocTy>::data()
{
        return _Data._First;
}

template<typename _AllocTy>
inline typename::_VectorBase<_AllocTy>::value_type* _VectorBase<_AllocTy>::data() const
{
        return _Data._First;
}

#include <allocators>

template<typename _Ty, typename _AllocTy = Allocator<_Ty>>
class Vector : public _VectorBase<_AllocTy>
{
public:
        typedef _VectorIterator<Vector, random_access_iterator_tag> iterator;
        typedef _VectorIterator<const Vector, random_access_iterator_tag> const_iterator;

        explicit Vector(const allocator_type& _Alloc = _AllocTy());
        explicit Vector(size_type _Size, const allocator_type& _Alloc = _AllocTy());
        Vector(size_type _Size, const value_type& _Val, const allocator_type& _Alloc = _AllocTy());
        ~Vector();      
        Vector& operator=(const Vector& _Right);

        size_type size() const noexcept;
        size_type max_size() const noexcept;
        void resize(size_type _Size);
        void resize(size_type _Size, const value_type& _Val);
        size_type capacity() const noexcept;
        bool empty() const noexcept;
        void reserve(size_type _Count);
        void shrink_to_fit();

        iterator       begin() noexcept;
        const_iterator cbegin() const noexcept;
        iterator       end() noexcept;
        const_iterator cend() const noexcept;

        reference       operator[](size_type _Idx);
        const_reference operator[](size_type _Idx) const;
        reference       at(size_type _Idx);
        const_reference at(size_type _Idx) const;
        reference       front();
        const_reference front() const;
        reference       back();
        const_reference back() const;

        iterator erase(const_iterator _Pos);
        void push_back(const value_type& _Val);
        void pop_back();
        void swap(Vector& _Vec);
        void clear() noexcept;
protected:
        void reallocate(size_type _Size, size_type _Capacity);
};

template<typename _Ty, typename _AllocTy>
inline Vector<_Ty, _AllocTy>::Vector(const allocator_type& _Alloc)
        : _VectorBase<_AllocTy>()
{
        resize(0);
}

template<typename _Ty, typename _AllocTy>
inline Vector<_Ty, _AllocTy>::Vector(size_type _Size, const allocator_type& _Alloc)
        : Vector<_Ty, _AllocTy>(_Alloc)
{
        resize(_Size);
}

template<typename _Ty, typename _AllocTy>
inline Vector<_Ty, _AllocTy>::Vector(size_type _Size, const value_type& _Val, const allocator_type& _Alloc)
        : Vector<_Ty, _AllocTy>(_Alloc)
{
        resize(_Size, _Val);
}

template<typename _Ty, typename _AllocTy>
inline Vector<_Ty, _AllocTy>::~Vector()
{
        clear();
}

template<typename _Ty, typename _AllocTy>
inline Vector<_Ty, _AllocTy>& Vector<_Ty, _AllocTy>::operator=(const Vector& _Right)
{
        return *this;
}

template<typename _Ty, typename _AllocTy>
inline typename::Vector<_Ty, _AllocTy>::size_type Vector<_Ty, _AllocTy>::size() const noexcept
{
        return _Data._Last - _Data._First;
}

template<typename _Ty, typename _AllocTy>
inline typename::Vector<_Ty, _AllocTy>::size_type Vector<_Ty, _AllocTy>::max_size() const noexcept
{
        _Assert(0); // not impemented yet
        return 0;
}

template<typename _Ty, typename _AllocTy>
inline void Vector<_Ty, _AllocTy>::resize(size_type _Size)
{
        if (_Data._First + _Size < _Data._End)
        {
                _Data._Last = _Data._First + _Size;
                return;
        }
        reallocate(_Size, 0);
}

template<typename _Ty, typename _AllocTy>
inline void Vector<_Ty, _AllocTy>::resize(size_type _Size, const value_type& _Val)
{
        size_type _Prev = size();
        resize(_Size);
        for (size_type i = _Prev; i < _Size; i++)
                *(_Data._First + i) = _Val;
}

template<typename _Ty, typename _AllocTy>
inline typename::Vector<_Ty, _AllocTy>::size_type Vector<_Ty, _AllocTy>::capacity() const noexcept
{
        return _Data._End - _Data._Last;
}

template<typename _Ty, typename _AllocTy>
inline bool Vector<_Ty, _AllocTy>::empty() const noexcept
{
        return _Data._First == _Data._Last;
}

template<typename _Ty, typename _AllocTy>
inline void Vector<_Ty, _AllocTy>::reserve(size_type _Count)
{
        reallocate(size(), _Count);
}

template<typename _Ty, typename _AllocTy>
inline void Vector<_Ty, _AllocTy>::shrink_to_fit()
{
        reallocate(size(), 0);
}

template<typename _Ty, typename _AllocTy>
inline typename::Vector<_Ty, _AllocTy>::iterator Vector<_Ty, _AllocTy>::begin() noexcept
{
        iterator it(_Data._First);
#if _DEBUG
        it._Vector = this;
#endif
        return it;
}

template<typename _Ty, typename _AllocTy>
inline typename::Vector<_Ty, _AllocTy>::const_iterator Vector<_Ty, _AllocTy>::cbegin() const noexcept
{
        const_iterator it(_Data._First);
#if _DEBUG
        it._Vector = this;
#endif
        return it;
}

template<typename _Ty, typename _AllocTy>
inline typename::Vector<_Ty, _AllocTy>::iterator Vector<_Ty, _AllocTy>::end() noexcept
{
        iterator it(_Data._Last);
#if _DEBUG
        it._Vector = this;
#endif
        return it;
}

template<typename _Ty, typename _AllocTy>
inline typename::Vector<_Ty, _AllocTy>::const_iterator Vector<_Ty, _AllocTy>::cend() const noexcept
{
        const_iterator it(_Data._Last);
#if _DEBUG
        it._Vector = this;
#endif
        return it;
}

template<typename _Ty, typename _AllocTy>
inline typename::Vector<_Ty, _AllocTy>::reference Vector<_Ty, _AllocTy>::operator[](size_type _Idx)
{
#if _DEBUG
        _Assert(_Idx < size());
#endif
        return *(_Data._First + _Idx);
}

template<typename _Ty, typename _AllocTy>
inline typename::Vector<_Ty, _AllocTy>::const_reference Vector<_Ty, _AllocTy>::operator[](size_type _Idx) const
{
#if _DEBUG
        _Assert(_Idx < size());
#endif
        return *(_Data._First + _Idx);
}

template<typename _Ty, typename _AllocTy>
inline typename::Vector<_Ty, _AllocTy>::reference Vector<_Ty, _AllocTy>::at(size_type _Idx)
{
#if _DEBUG
        _Assert(_Idx < size());
#endif
        return *(_Data._First + _Idx);
}

template<typename _Ty, typename _AllocTy>
inline typename::Vector<_Ty, _AllocTy>::const_reference Vector<_Ty, _AllocTy>::at(size_type _Idx) const
{
#if _DEBUG
        _Assert(_Idx < size());
#endif
        return *(_Data._First + _Idx);
}

template<typename _Ty, typename _AllocTy>
inline typename::Vector<_Ty, _AllocTy>::reference Vector<_Ty, _AllocTy>::front()
{
        return *_Data._First;
}

template<typename _Ty, typename _AllocTy>
inline typename::Vector<_Ty, _AllocTy>::const_reference Vector<_Ty, _AllocTy>::front() const
{
        return *_Data._First;
}

template<typename _Ty, typename _AllocTy>
inline typename::Vector<_Ty, _AllocTy>::reference Vector<_Ty, _AllocTy>::back()
{
        return *(_Data._Last - 1);
}

template<typename _Ty, typename _AllocTy>
inline typename::Vector<_Ty, _AllocTy>::const_reference Vector<_Ty, _AllocTy>::back() const
{
        return *(_Data._Last - 1);
}

template<typename _Ty, typename _AllocTy>
inline typename::Vector<_Ty, _AllocTy>::iterator Vector<_Ty, _AllocTy>::erase(const_iterator _Pos)
{
        pointer _Start = _Pos._Ptr;
        iterator it(_Start + 1);
#if _DEBUG
        *_Pos; // in-range check
        it._Vector = this;
#endif
        pointer _End = --_Data._Last;
        for (pointer i = _Start; i < _End; i++)
                *i = *(i + 1);
        return it;
}

template<typename _Ty, typename _AllocTy>
inline void Vector<_Ty, _AllocTy>::push_back(const value_type& _Val)
{
        size_type d = _Data._End - _Data._First;
        if (_Data._Last == _Data._End)
                reserve(1 + (_Data._End - _Data._First) / 2);
        *(_Data._Last++) = _Val;
}

template<typename _Ty, typename _AllocTy>
inline void Vector<_Ty, _AllocTy>::pop_back()
{
        if (_Data._Last > _Data._First)
                _Data._Last--;
}

template<typename _Ty, typename _AllocTy>
inline void Vector<_Ty, _AllocTy>::swap(Vector& _Vec)
{
        _VectorBase<_AllocTy>::swap(_Vec);
}

template<typename _Ty, typename _AllocTy>
inline void Vector<_Ty, _AllocTy>::clear() noexcept
{
        size_type _Total = _Data._End - _Data._First;
        for (size_type i = 0; i < _Total; i++)
                get_allocator().destroy(_Data._First + i);
        get_allocator().deallocate(_Data._First, _Total);
        _Data._First = pointer();
        _Data._Last = pointer();
        _Data._End = pointer();
}

template<typename _Ty, typename _AllocTy>
inline void Vector<_Ty, _AllocTy>::reallocate(size_type _Size, size_type _Capacity)
{
        pointer _New = get_allocator().allocate(_Size + _Capacity);
        size_type _Total = _Data._End - _Data._First;
        size_type _New_total = _Size + _Capacity;
        size_type _Min = _Total > _New_total ? _New_total : _Total;

        for (size_type i = 0; i < _Min; i++)
                get_allocator().construct(_New + i, *(_Data._First + i));
        for (size_type i = _Min; i < _Total; i++)
                get_allocator().destroy(_Data._First + i);
        for (size_type i = _Min; i < _New_total; i++)
                get_allocator().construct(_New + i);

        get_allocator().deallocate(_Data._First, _Total);
        _Data._First = _New;
        _Data._Last = _Data._First + _Size;
        _Data._End = _Data._Last + _Capacity;
}
