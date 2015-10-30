#pragma once 

#include "variant_lite.hpp"

namespace std          {
namespace experimental {
namespace parallel     {
inline namespace v1    {


template<class DefaultPolicy, class... Policies>
class __dynamic_execution_policy
{
    private:
        __variant_lite<DefaultPolicy, Policies...> policy_;

        template<class ExecutionPolicy>
        bool is_policy() const { return policy_.template is_type<ExecutionPolicy>(); }

    public:  

        template<typename ExecutionPolicy>
        __dynamic_execution_policy(const ExecutionPolicy& exec)  : policy_(exec)
        {}

        template<class ExecutionPolicy>
        typename enable_if<is_execution_policy<ExecutionPolicy>::value, ExecutionPolicy*>::type
        get()  __NOEXCEPT
        {
            return policy_.template get<ExecutionPolicy>();
        }
        template<class ExecutionPolicy>
        typename enable_if<is_execution_policy<ExecutionPolicy>::value, const ExecutionPolicy*>::type
        get()  const __NOEXCEPT
        {
            return policy_.template get<ExecutionPolicy>();
        }

        const type_info& type() const __NOEXCEPT
        {
            return policy_.type();
        }

    private:

        // algorithm dispatch : loop over policies to detect the one that needs to be dispatched
        //
        template<class...>
        struct policies_placeholder{};

        template<class... Args>
        decltype(declval<DefaultPolicy>().dispatch(declval<Args>()...))
        dispatch(policies_placeholder<>&&, Args&&... args) const 
        {
            return get<DefaultPolicy>()->dispatch(forward<Args>(args)...);
        }

        template<class ExecutionPolicy, class... Execs, class... Args>
        decltype(declval<DefaultPolicy>().dispatch(declval<Args>()...))
        dispatch(policies_placeholder<ExecutionPolicy,Execs...>&&, Args&&... args) const 
        {
            if (is_policy<ExecutionPolicy>())
            {
                return get<ExecutionPolicy>()->dispatch(forward<Args>(args)...);
            }
            else
            {
                return dispatch(policies_placeholder<Execs...>{}, forward<Args>(args)...);
            }
        }

    public:
                
        // algorithm dispatch
        //
        template<class... Args>
        decltype(declval<DefaultPolicy>().dispatch(declval<Args>()...))
        dispatch(Args&&... args) const 
        {
            return dispatch(policies_placeholder<DefaultPolicy, Policies...>{}, forward<Args>(args)...);
        }
};

struct execution_policy
{
    private:
        __dynamic_execution_policy<
            sequential_execution_policy,
            parallel_execution_policy
        > policy_;
    public:
        // 2.7.1, execution_policy construct/assign
        template<class ExecutionPolicy> 
        execution_policy(const ExecutionPolicy& policy,
                         typename enable_if<is_execution_policy<ExecutionPolicy>::value>::type * = 0) : policy_(policy)
        {}

        template<class ExecutionPolicy>
        typename enable_if<is_execution_policy<ExecutionPolicy>::value,execution_policy&>::type
        operator=(const ExecutionPolicy& policy)
        {
            policy_ = policy;
            return *this;
        }

        // 2.7.2, execution_policy object access
        const type_info& type() const __NOEXCEPT
        {
            return policy_.type();
        }

        template<class ExecutionPolicy>
        ExecutionPolicy* get() __NOEXCEPT
        {
            return policy_.get<ExecutionPolicy>();
        }
        template<class ExecutionPolicy>
        const ExecutionPolicy* get() const __NOEXCEPT
        {
            return policy_.get<ExecutionPolicy>();
        }

        // algorithm dispatch
        //
        template<class... Args>
        auto dispatch(Args&&... args) const  ->
        decltype(policy_.dispatch(forward<Args>(args)...))
        {
            return policy_.dispatch(forward<Args>(args)...);
        }
};


}
}
}
}

