#pragma once

#include <tuple>
#include <type_traits>
#include <utility>

#include "reactions/resource.h"

namespace reactions
{

// 前向声明
template<typename T, typename... Args>
class DataSource;


template<typename T>
struct ExpressionTraits
{
    using type = T;
};

// 偏特化
template<typename T>
struct ExpressionTraits<DataSource<T>>
{
    using type = T;
};

template<typename Func, typename... Args>
struct ExpressionTraits<DataSource<Func, Args ...>>
{
    using type = std::invoke_result_t<Func, typename ExpressionTraits<Args>::type...>;
};

template<typename T, typename... Args>
using ReturnType = typename ExpressionTraits<DataSource<T, Args...>>::type;






template<typename Func, typename... Args>
class Expression : public Resource<ReturnType<Func, Args...>>
{
public:
    using value_type = ReturnType<Func, Args...>;

    using Resource<value_type>::Resource;

    template<typename F, typename... As>
    Expression(F&& f, As&&... args)
        : Resource<value_type>() 
        , func_(std::forward<F>(f))
        , args_(std::forward<As>(args)...)
    {
        evaluate();
    }


    void evaluate()
    {
        auto result = [&]<std::size_t... I>(std::index_sequence<I...>)
        {
            return std::invoke(func_, std::get<I>(args_).get().get()...);
        } (std::make_index_sequence<std::tuple_size_v<decltype(args_)>>{});

        this->updateValue(result);
    }


private:
    Func func_;
    std::tuple<std::reference_wrapper<Args>...> args_;
};

template<typename Type>
class Expression<Type> : public Resource<Type>
{
public:
    using value_type = Type;
    
    using Resource<value_type>::Resource;

};


} // namespace reactions