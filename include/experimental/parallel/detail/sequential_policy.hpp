#pragma once

#include <algorithm>



namespace std          {
namespace experimental {
namespace parallel     {
inline namespace v1    {

struct sequential_execution_policy
{
    template<class InputIterator, class Function>
    static void for_each(InputIterator first, InputIterator last, Function f)
    {
        std::for_each(first, last, f);
    }
};



}
}
}
}
