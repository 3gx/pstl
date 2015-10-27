#pragma once

#ifdef _OPENMP
#include <parallel/algorithm>


namespace std          {
namespace experimental {
namespace parallel     {
inline namespace v1    {

class parallel_execution_policy
{
    template<class InputIterator, class Function>
    friend void dispatch(const detail::for_each&, const parallel_execution_policy &par, 
                         InputIterator first, InputIterator last, Function f) 
    {
        __gnu_parallel::for_each(first, last, f);
    }
};


}
}
}
}
#else
#include <algorithm>


namespace std          {
namespace experimental {
namespace parallel     {
inline namespace v1    {

class parallel_execution_policy
{
    template<class InputIterator, class Function>
    friend void dispatch(detail::for_each&&, const parallel_execution_policy &par, 
                         InputIterator first, InputIterator last, Function f) 
    {
        std::for_each(first, last, f);
    }
};


}
}
}
}
#endif
