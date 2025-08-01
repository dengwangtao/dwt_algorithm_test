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


struct VarExprTag {};
struct CalcExprTag {};





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
    using SuperClass = Resource<value_type>;
    using SuperClass::Resource;

    using ExprType = CalcExprTag;

    template<typename F, typename... As>
    Expression(F&& f, As&&... args)
        : Resource<value_type>() 
        , func_(std::forward<F>(f))
        , args_(std::forward<As>(args)...)
    {
        this->subscribe(std::forward<Args>(args)...);
        
        evaluate();
    }

private:

    void evaluate()
    {
        auto result = [&]<std::size_t... I>(std::index_sequence<I...>)
        {
            return std::invoke(func_, std::get<I>(args_).get().get()...);
        } (std::make_index_sequence<std::tuple_size_v<decltype(args_)>>{});

        this->updateValue(result);
    }

    void valueChanged() override
    {
        this->evaluate();
        this->notify();
    }


private:
    Func func_;
    std::tuple<std::reference_wrapper<Args>...> args_;

    // 友元声明
    template<typename OS, typename T, typename... As>
    friend OS& operator<<(OS& os, const Expression<T, As...>& expr);
};



template<typename Type>
class Expression<Type> : public Resource<Type>
{
public:
    using value_type = Type;
    
    using Resource<value_type>::Resource;

    using ExprType = VarExprTag;

};



template<typename OS, typename T, typename... Args>
OS& operator<<(OS& os, const Expression<T, Args...>& expr)
{
    os  << "Expr {" << "addr: " << (void*)&expr << " value=" << expr.getValue()
        << " dependencies: ";

    // 检测是否有成员 args_（编译期）
    if constexpr (requires { expr.args_; })
    {
        os << "[";
        [&]<std::size_t... I>(std::index_sequence<I...>)
        {
            (void)((os << (I == 0 ? "" : ", ") << std::get<I>(expr.args_).get()),...);
        } (std::make_index_sequence<std::tuple_size_v<decltype(expr.args_)>>{});
        os << "]";
    }
    else
    {
        os << "None";
    }
    os << "}";

    return os;
}


} // namespace reactions