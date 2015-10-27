#pragma once

#ifdef _OPENMP
#include <parallel/algorithm>
#define PARSPACE __gnu_parallel
#else
#include <algorithm>
#define PARSPACE std
#endif


namespace std          {
namespace experimental {
namespace parallel     {
inline namespace v1    {

class parallel_execution_policy
{
    template<class InputIterator, class Function>
    friend void dispatch(const details::for_each&, const parallel_execution_policy &par, 
                         InputIterator first, InputIterator last, Function f) 
    {
        PARSPACE::for_each(first, last, f);
    }

    template<class RandomIt>
    friend void dispatch(const details::sort&, const parallel_execution_policy &seq,
                         RandomIt first, RandomIt last)
    {
        PARSPACE::sort(first, last);
    }
};

const static parallel_execution_policy par{};


}
}
}
}
