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

struct parallel_execution_policy
{
    // for_each
    //
    template<class InputIterator, class Function>
    void dispatch(__for_each&&,
                  InputIterator first, InputIterator last, Function f)  const
    {
        PARSPACE::for_each(first, last, f);
    }

    // for_each_n
    //
    template<class InputIt, class Size, class Unary>
    InputIt dispatch(__for_each_n&&,
                     InputIt first, Size n, Unary f)  const
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
    void dispatch(__sort&&,
                  RandomIt first, RandomIt last) const
    {
        PARSPACE::sort(first, last);
    }
};

const static parallel_execution_policy par{};


}
}
}
}
