#pragma once

#include <algorithm>



namespace std          {
namespace experimental {
namespace parallel     {
inline namespace v1    {

class sequential_execution_policy
{
    template<class InputIterator, class Function>
    friend void dispatch(const detail::for_each&, const sequential_execution_policy &seq, 
                         InputIterator first, InputIterator last, Function f) 
    {
        std::for_each(first, last, f);
    }
};



}
}
}
}
