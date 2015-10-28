#pragma once

#include <algorithm>



namespace std          {
namespace experimental {
namespace parallel     {
inline namespace v1    {

struct sequential_execution_policy
{
    // for_each
    //
    template<class InputIterator, class Function>
    void dispatch(__for_each&&,
                  InputIterator first, InputIterator last, Function f)  const
    {
        std::for_each(first, last, f);
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
        std::for_each(first, last, f);
        return last;
    }  

    // sort
    //
    template<class RandomIt>
    void dispatch(__sort&&,
                  RandomIt first, RandomIt last) const
    {
        std::sort(first, last);
    }
};

const static sequential_execution_policy seq{};

}
}
}
}
