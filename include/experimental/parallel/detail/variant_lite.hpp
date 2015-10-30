#pragma once

#include <typeinfo>
#include <memory>
#include <cassert>

namespace std          {
namespace experimental {
namespace parallel     {
inline namespace v1    {

template<bool I, bool... Js>
struct __constexpr_or
{
    static constexpr auto value = I | __constexpr_or<Js...>::value;
};
template<bool I>
struct __constexpr_or<I>
{
    static constexpr auto value = I;
};



template<int i, int... js>
struct __constexpr_max
{
    static const auto value = i < __constexpr_max<js...>::value ? __constexpr_max<js...>::value : i;
};
template<int i>
struct __constexpr_max<i>
{
    static const auto value = i;
};

template<class... Ts>
class __variant_lite
{
    private:
        using storage_type =  typename std::aligned_storage<
            __constexpr_max<sizeof(Ts)...>::value
                >::type;
        storage_type storage_;
        const type_info *type_;

        template<class...>
        struct type_placeholder{};

        using all_types = type_placeholder<Ts...>;

        template<typename V>
        void construct(const V& v)
        {
            auto ptr = reinterpret_cast<V*>(&storage_);
            new (ptr) V(v);
            type_ = &typeid(v);
        }

        void construct(type_placeholder<>&&, const __variant_lite&)
        { assert(false); }
        template<class V, class... Vs>
        void construct(type_placeholder<V,Vs...>&&, const __variant_lite& rhs)
        {
            if (rhs.is_type<V>())
            {
                construct(*rhs.get<V>());
            }
            else
            {
                construct(type_placeholder<Vs...>{}, rhs);
            }
        }

        
    public:

        template<class V>
        bool is_type() const 
        {
            return 
                typeid(V).hash_code() == type_->hash_code(); 
        }

        template<typename V>
        explicit __variant_lite(const V& v)
        {
            static_assert(
                    __constexpr_or<is_same<V,Ts>::value...>::value, 
                    "Type mismatch in __variant_lite ctor");
            construct(v);
        }

        explicit __variant_lite(const __variant_lite& rhs) 
        {
            construct(all_types{}, rhs);
        }

        template<class V>
        V* get()  __NOEXCEPT
        {
            if (!is_type<V>())
            {
                return nullptr;
            }
            return reinterpret_cast<V*>(&storage_);
        }
        template<class V>
        const V* get() const __NOEXCEPT
        {
            if (!is_type<V>())
            {
                return nullptr;
            }
            return reinterpret_cast<const V*>(&storage_);
        }
        const type_info& type() const __NOEXCEPT
        {
            return *type_;
        }


    private:



        void destroy(type_placeholder<>&&)
        { assert(false); }
        template<class V, class... Vs>
        void destroy(type_placeholder<V,Vs...>&&)
        {
            if (is_type<V>())
            {
                get<V>()->~V();
            }
            else
            {
                destroy(type_placeholder<Vs...>{});
            }
        }


        void copy(type_placeholder<>&&, const __variant_lite&)
        { assert(false); }
        template<class V, class... Vs>
        void copy(type_placeholder<V,Vs...>&&, const __variant_lite& rhs)
        {
            if (rhs.is_type<V>())
            {
                this->operator=(*rhs.get<V>()); 
            }
            else
            {
                copy(type_placeholder<Vs...>{}, rhs);
            }
        }

    public:

        ~__variant_lite()
        {
            destroy(all_types{});
        }

        template<typename V>
        __variant_lite& operator=(const V& rhs)
        {
            static_assert(
                    __constexpr_or<is_same<V,Ts>::value...>::value, 
                    "Type mismatch in __variant_lite copy asignment operator");
            if (is_type<V>())
            {
                *get<V>() = rhs;
            }
            else
            {
                destroy(all_types{});
                construct(rhs);
            }
            return *this;
        }

        __variant_lite& operator=(const __variant_lite& rhs)
        {
            copy(all_types{},rhs);
            return *this;
        }

};

}
}
}
}
