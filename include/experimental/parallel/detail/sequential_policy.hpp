#pragma once

#include <algorithm>



namespace std          {
namespace experimental {
namespace parallel     {
inline namespace v1    {

class sequential_execution_policy
{
    // for_each
    //
    template<class InputIterator, class Function>
    friend void dispatch(const __for_each&, const sequential_execution_policy &seq, 
                         InputIterator first, InputIterator last, Function f) 
    {
        std::for_each(first, last, f);
    }
 
    // for_each_n
    //
    template<class InputIt, class Size, class Unary>
    friend InputIt dispatch(const __for_each_n&, const sequential_execution_policy &seq, 
                         InputIt first, Size n, Unary f) 
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
    friend void dispatch(const __sort&, const sequential_execution_policy &seq,
                         RandomIt first, RandomIt last)
    {
        std::sort(first, last);
    }
};

const static sequential_execution_policy seq{};

}
}
}
}
