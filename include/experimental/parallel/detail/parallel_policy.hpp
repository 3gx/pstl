#pragma once

#include <parallel/algorithm>


namespace std          {
namespace experimental {
namespace parallel     {
inline namespace v1    {

struct parallel_execution_policy
{
    template<class InputIterator, class Function>
    static void for_each(InputIterator first, InputIterator last, Function f)
    {
        __gnu_parallel::for_each(first, last, f);
    }
};




}
}
}
}
