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
    // for_each
    //
    template<class InputIterator, class Function>
    friend void dispatch(const __for_each&, const parallel_execution_policy &par, 
                         InputIterator first, InputIterator last, Function f) 
    {
        PARSPACE::for_each(first, last, f);
    }

    // for_each_n
    //
    template<class InputIt, class Size, class Unary>
    friend InputIt dispatch(const __for_each_n&, const parallel_execution_policy &par,
                         InputIt first, Size n, Unary f) 
    {
        if (n < 0)
            return first;
        auto last = first;
        std::advance(last, n);
        PARSPACE::for_each(first, last, f);
        return last;
    }  

    // sort
    //
    template<class RandomIt>
    friend void dispatch(const __sort&, const parallel_execution_policy &par,
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
