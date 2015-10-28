#pragma once 
namespace std          {
namespace experimental {
namespace parallel     {
inline namespace v1    {

class dynamic_execution_policy
{
    private:
        union 
        {
            sequential_execution_policy seq_;
            parallel_execution_policy   par_;
        };
        enum class policy_type
        {
            SEQ, PAR
        } policy_;

    public:
        dynamic_execution_policy(const sequential_execution_policy& seq) : seq_(seq), policy_(policy_type::SEQ) {}
        dynamic_execution_policy(const   parallel_execution_policy& par) : par_(par), policy_(policy_type::PAR) {}
        dynamic_execution_policy(const    dynamic_execution_policy&) = default;
        dynamic_execution_policy& operator=(const dynamic_execution_policy&) = default;

        bool is_seq() const { return policy_ == policy_type::SEQ; }
        bool is_par() const { return policy_ == policy_type::PAR; }

        template<class ExecutionPolicy>
        typename enable_if<is_execution_policy<ExecutionPolicy>::value, ExecutionPolicy*>::type
        get()  __NOEXCEPT
        {
            ExecutionPolicy* dummy = nullptr;
            return get_helper(*dummy);
        }
        template<class ExecutionPolicy>
        typename enable_if<is_execution_policy<ExecutionPolicy>::value, const ExecutionPolicy*>::type
        get()  const __NOEXCEPT
        {
            ExecutionPolicy* dummy = nullptr;
            return get_helper(dummy);
        }

        // sequential_execution_policy  get_helper
        //
        sequential_execution_policy* get_helper(sequential_execution_policy*) __NOEXCEPT
        {
            return is_seq() ? &seq_ : nullptr;
        }
        const sequential_execution_policy* get_helper(sequential_execution_policy*) const __NOEXCEPT
        {
            return is_seq() ? &seq_ : nullptr;
        }

        // parallel_execution_policy  get_helper
        //
        parallel_execution_policy* get_helper(parallel_execution_policy*) __NOEXCEPT
        {
            return is_par() ? &par_ : nullptr;
        }
        const parallel_execution_policy* get_helper(parallel_execution_policy*) const __NOEXCEPT
        {
            return is_par() ? &par_ : nullptr;
        }

        // type information
        //
        const type_info& type() const __NOEXCEPT
        {
            if (is_seq()) return typeid(sequential_execution_policy);
            if (is_par()) return typeid(  parallel_execution_policy);
            return typeid(nullptr);
        }

        // algorithm dispatch
        //
        template<class Functor, class... Args>
        auto dispatch(Functor&& f, Args&&... args) const -> decltype(f(seq_, std::forward<Args>(args)...))
        {
            if (is_seq()) return f(seq_, std::forward<Args>(args)...);
            if (is_par()) return f(par_, std::forward<Args>(args)...);
            return f(seq_, args...);
        }
};

class execution_policy
{
    private:
        dynamic_execution_policy policy_;
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

        const dynamic_execution_policy& get_policy() const { return policy_; }
};

// algorithm dispatch
//
template<class Functor, class... Args>
auto dispatch(Functor&& f, const execution_policy &exec, Args&&... args) ->
decltype(exec.get_policy().dispatch(std::forward<Functor>(f), std::forward<Args>(args)...))
{
    return exec.get_policy().dispatch(std::forward<Functor>(f), std::forward<Args>(args)...);
}

}
}
}
}

