#pragma once 
namespace std          {
namespace experimental {
namespace parallel     {
inline namespace v1    {

class policy_union
{
    friend class execution_policy;
    private:
        union 
        {
            sequential_execution_policy seq_;
            parallel_execution_policy par_;
        };
        enum class policy_type
        {
            SEQ, PAR
        } policy_;

    public:
        policy_union(const sequential_execution_policy& seq) : seq_(seq), policy_(policy_type::SEQ) {}
        policy_union(const parallel_execution_policy&   par) : par_(par), policy_(policy_type::PAR) {}

        bool is_seq() const { return policy_ == policy_type::SEQ; }
        bool is_par() const { return policy_ == policy_type::PAR; }

        template<class ExecutionPolicy>
        typename enable_if<is_execution_policy<ExecutionPolicy>::value, ExecutionPolicy*>::type
        get()  __NOEXCEPT
        {
            ExecutionPolicy* dummy;
            return get_helper(*dummy);
        }
        template<class ExecutionPolicy>
        typename enable_if<is_execution_policy<ExecutionPolicy>::value, const ExecutionPolicy*>::type
        get()  const __NOEXCEPT
        {
            ExecutionPolicy* dummy;
            return get_helper(*dummy);
        }

        /* sequential_execution_policy */
        sequential_execution_policy* get_helper(sequential_execution_policy*) __NOEXCEPT
        {
            return is_seq() ? &seq_ : nullptr;
        }
        const sequential_execution_policy* get_helper(sequential_execution_policy*) const __NOEXCEPT
        {
            return is_seq() ? &seq_ : nullptr;
        }
        policy_union& operator=(const sequential_execution_policy& seq)
        {
            *this = policy_union(seq);
            return *this;
        }

        /* parallel_execution_policy */
        parallel_execution_policy* get_helper(parallel_execution_policy*) __NOEXCEPT
        {
            return is_par() ? &par_ : nullptr;
        }
        const parallel_execution_policy* get_helper(parallel_execution_policy*) const __NOEXCEPT
        {
            return is_par() ? &par_ : nullptr;
        }
        policy_union& operator=(const parallel_execution_policy& par)
        {
            *this   = policy_union(par);
            return *this;
        }

        const type_info& type() const __NOEXCEPT
        {
            if (is_seq()) return typeid(sequential_execution_policy);
            if (is_par()) return typeid(parallel_execution_policy);
        }


        template<class InputIterator, class Function>
        void for_each( InputIterator first, InputIterator last, Function f) const 
        {
            if (is_seq()) return __for_each(seq_, first, last, f);
            if (is_par()) return __for_each(par_, first, last, f);
        }
};

class execution_policy
{
    private:
        policy_union policy_;
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

        template<class InputIterator, class Function>
        friend void __for_each(const execution_policy &exec, InputIterator first, InputIterator last, Function f)
        {
            exec.policy_.for_each(first,last,f);
        }

};

}
}
}
}

