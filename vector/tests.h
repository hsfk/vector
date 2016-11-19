#pragma once

#include "vector.h"
#include <cstdlib>
#include <assert.h>
#include <iostream>
#include <vector>
#include <ctime>
#include <algorithm>

#define _ok(test_name) std::cout << "ok " << #test_name << std::endl
#define _not_ok(test_name) std::cout << "line: " << __LINE__ << " not ok " << #test_name << std::endl
#define ok(expr, test_name)\
        if (expr)\
                _ok(test_name);\
        else\
                _not_ok(test_name)
#define ok_if_zero_exception_caught(test_name)\
        catch(...) { _not_ok(test_name); } _ok(test_name)

template <typename _Vec, typename _MyVec>
bool equal(const _Vec& _Vector, const _MyVec& _MyVector)
{
        if (_Vector.size() != _MyVector.size())
                return false;
        for (size_t i = 0; i < _Vector.size(); i++)
        {
                int  a = _Vector[i];
                a = _MyVector[i];
                if (_Vector[i] != _MyVector[i])
                        return false;
        }
        return true;
}

void test_constructors()
{
        try
        {
                {
                        Vector<int> a;
                        ok(a.empty(), empty);
                }
                {
                        Vector<int> a(100, 100);
                        std::vector<int>b(100, 100);
                        ok(equal(b, a), fill + resize);
                }
        }
        ok_if_zero_exception_caught(constructor tests);
}

void test_iterators()
{
        {
                Vector<int> a;
                a.push_back(19);
                ok(*a.begin() == 19, begin);
                ok(*(--a.end()) == 19, end);
                ok(a.begin() != a.end(), iterator not equal);
        }
        {
                Vector<int> a(100, 100);   
                Vector<int>::iterator it;
                try
                {
                        it = a.begin();
                        for (int i = 0; i < 100; i++)
                                _Assert(*it++ == 100);
                }
                ok_if_zero_exception_caught(iterator postfix increment);
                try
                {
                        it = a.begin();
                        for (int i = 1; i < 100; i++)
                                _Assert(*++it == 100);
                }
                ok_if_zero_exception_caught(iterator prefix increment);
                try
                {
                        it = a.end();
                        for (int i = 0; i < 100; i++)
                                _Assert(*--it == 100);
                }
                ok_if_zero_exception_caught(iterator prefix decrement);
                try
                {
                        it = a.end();
                        it--;
                        for (int i = 1; i < 100; i++)
                                _Assert(*it-- == 100);
                }
                ok_if_zero_exception_caught(iterator postfix decrement);
        }
}

void test_other()
{
        {
                Vector<int> a;
                std::vector<int> b;
                for (int i = 0; i < 1000; i++)
                {
                        a.push_back(i);
                        b.push_back(i);
                }
                ok(equal(b, a), push back);
        }
        {
                Vector<int> a(100, 100);
                std::vector<int> b(100, 100);
                for (int i = 0; i < 10; i++)
                {
                        a.pop_back();
                        b.pop_back();
                }
                ok(equal(b, a), pop back);
        }
        {
                Vector<int> a;
                a.push_back(10);
                a.reserve(100);
                a.shrink_to_fit();
                ok(a.front() == 10 && a.capacity() == 0, shrink to fit);
        }
        {
                Vector<int> a(1, 1);
                ok(a.back() == 1, back);
                ok(a[0] == 1, zero element access);
        }
        {
                Vector<int> a(100, 100);
                a.push_back(3);
                ok(a[100] == 3, last element access);
                a.reserve(10);
        }
        {
                Vector<int> a(100, 100);
                std::vector<int> b(100, 100);
                a.resize(10, 10);
                b.resize(10, 10);
                ok(equal(b, a), resize);
        }
        {
                Vector<int> a(100, 100);
                std::vector<int> b(100, 100);
                Vector<int>::const_iterator ait = a.cbegin();
                std::vector<int>::const_iterator bit = b.cbegin();
                for (int i = 0; i < 30; i++)
                {
                        ait++;
                        bit++;
                }
                a.erase(ait);
                b.erase(bit);
                ok(equal(b, a), erase);
        }
}

void random_tests()
{
        srand(time(NULL));
        Vector<int> a;
        std::vector<int> b;
        for (int i = 0; i < 30000; i++)
        {
                unsigned r = rand();
                for (int i = 0; i < r % 100; i++)
                {
                        a.push_back(i);
                        b.push_back(i);
                }
                r = rand();
                for (int i = 0; i < r % 100; i++)
                        if (b.size() > 0)
                        {
                                a.pop_back();
                                b.pop_back();
                        }
                if (r % 10 == 7)
                {
                        a.clear();
                        b.clear();
                }
                else if (r % 10 == 0)
                {
                        a.reserve(10);
                        b.reserve(10);
                }
                else if (r % 50 == 13)
                {
                        a.resize(10, 0);
                        b.resize(10, 0);
                        a.shrink_to_fit();
                        b.shrink_to_fit();
                }
        }
        ok(equal(b, a), random tests);
}

class c
{
public:
        c()
        {
                ptr = new int[1000];
                for (size_t i = 0; i < 1000; i++)
                        ptr[i] = 123;
        }

        ~c()
        {
                delete[] ptr;
        }
        int* ptr;
};

#if !_DEBUG
        #define STL_COMPATIBILITY
#endif

void stl_compatibility()
{
#ifdef STL_COMPATIBILITY
        srand(time(NULL));
        {
                Vector<int, std::allocator<int>> a(10, 10);
                a.clear();
                a.reserve(100);
                for (int i = 0; i < 100; i++)
                        a.push_back(100);
                ok(1, std::allocator);
        }
        {
                Vector<int, std::allocator<int>> a;
                for (int i = 0; i < 1000; i++)
                        a.push_back(rand());
                std::sort(a.begin(), a.end());
                bool result = true;
                for (int i = 0; i < 999; i++)
                        result &= a[i] <= a[i + 1];
                ok(result, std::sort);
        }
#endif
}

void test_objects()
{
        try
        {
                {
                        Vector<c> a(1);
                        bool result = true;
                        for (size_t i = 0; i < 100; i++)
                                result &= a[0].ptr[i] == 123;
                        ok(result, object initialization);
                }
                {
                        for (size_t i = 0; i < 100; i++)
                                Vector<c> a(10);
                }
                {
                        for (size_t i = 0; i < 100; i++)
                        {
                                Vector<c> a;
                                a.reserve(50);
                                a.resize(20);
                                a.clear();
                        }
                }
                {
                        for (size_t i = 0; i < 100; i++)
                        {
                                Vector<c, std::allocator<c>> a;
                                a.reserve(50);
                                a.resize(20);
                                a.clear();
                        }
                }
        }
        ok_if_zero_exception_caught(object tests);
}

template <typename T>
void test_allocator_perfomance(T allocator, std::string alloc_name)
{
        const int complexity = 20000;
        clock_t begin = clock();

        Vector<int, T> v(allocator);

        for (int i = 0; i < complexity; i++)
                v.resize(100);
        v.clear();
        for (int i = 0; i < complexity; i++)
                v.push_back(0);
        v.clear();

        Vector<int, T> v2(allocator);
        for (int i = 0; i < complexity; i++)
                if (i % 2 == 0)
                        v.push_back(0);
                else
                        v2.push_back(0);
        v2.clear();
        v.clear();

        std::cout << "\n" << alloc_name.c_str() << " perfomance: " << float(clock() - begin);
}

void test()
{
        test_constructors();
        test_iterators();
        test_other();
        test_objects();
        stl_compatibility();
        random_tests();

        test_allocator_perfomance(Allocator<int, first_fit_pool_tag>(), "first fit allocator");
        test_allocator_perfomance(Allocator<int, best_fit_pool_tag>(), "best fit allocator");
        test_allocator_perfomance(Allocator<int, worst_fit_pool_tag>(), "worst fit allocator");
        //test_allocator_perfomance(Allocator<int, next_fit_pool_tag>(), "next fit allocator");
        test_allocator_perfomance(std::allocator<int>(), "std::allocator");
}
