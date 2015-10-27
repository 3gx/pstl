#pragma once

#include <parallel/algorithm>


namespace std          {
namespace experimental {
namespace parallel     {
inline namespace v1    {

struct parallel_execution_policy
{
    template<class InputIterator, class Function>
    friend void __for_each(const parallel_execution_policy &par, InputIterator first, InputIterator last, Function f) 
    {
        __gnu_parallel::for_each(first, last, f);
    }
};




}
}
}
}
