#pragma once

#include <tuple>
#include <type_traits>
#include <utility>

#include "reactions/resource.h"

namespace reactions
{
struct VarExprTag {};
struct CalcExprTag {};



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
        // , func_(std::forward<F>(f))
        // , args_(std::forward<As>(args)...)
    {
        // this->subscribe(std::forward<Args>(args)...);
        
        // evaluate();
        setSource(std::forward<F>(f), std::forward<As>(args)...);
    }

    template<typename F, typename... As>
    void setSource(F&& f, As&&... args)
    {
        using new_value_type = ReturnType<std::decay_t<F>, std::decay_t<As>...>;

        static_assert(std::is_convertible_v<new_value_type, value_type>, "return type mismatch");

        if constexpr (std::is_convertible_v<new_value_type, value_type>)
        {
            this->subscribe(std::forward<As>(args)...);

            args_func_ = createArgsFunc(std::forward<F>(f), std::forward<As>(args)...);

            evaluate();
        }        
    }

private:

    template<typename F, typename... As>
    auto createArgsFunc(F&& f, As&&... args)
    {
        return [func = std::forward<F>(f), ...args_ = args.getImpl()]() mutable
        {
            if constexpr (IsVoidConcept<value_type>)
            {
                std::invoke(func, args_->get()...);
                return VoidWrapper{};
            }
            else
            {
                return std::invoke(func, args_->get()...);
            }
        };
    }

    void evaluate()
    {

        // 判断value_type是否为void
        // IsVoidConcept
        if constexpr (IsVoidConcept<value_type>)
        {
            // [&]<std::size_t... I>(std::index_sequence<I...>)
            // {
            //     std::invoke(func_, std::get<I>(args_).get().get()...);
            // } (std::make_index_sequence<std::tuple_size_v<decltype(args_)>>{});
            // return;
            std::invoke(args_func_);
        }
        else
        {
            // auto result = [&]<std::size_t... I>(std::index_sequence<I...>)
            // {
            //     return std::invoke(func_, std::get<I>(args_).get().get()...);
            // } (std::make_index_sequence<std::tuple_size_v<decltype(args_)>>{});

            auto result = std::invoke(args_func_);

            this->updateValue(result);
        }
    }

    void valueChanged() override
    {
        this->evaluate();
        // this->notify();
        SuperClass::valueChanged();
    }

private:
    // Func func_;
    // std::tuple<std::reference_wrapper<Args>...> args_;
    std::function<value_type()> args_func_; // 捕获args_


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