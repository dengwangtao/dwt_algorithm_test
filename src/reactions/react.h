#pragma once

#include <atomic>

#include "reactions/expression.h"

namespace reactions
{

template<typename Type, typename... Args>
class ReactImpl : public Expression<Type, Args...>
{
public:
    using SuperClass = Expression<Type, Args...>;
    using SuperClass::Expression;
    using value_type = SuperClass::value_type;

    using ExprType = SuperClass::ExprType;


    auto get() const
    {
        return this->getValue();
    }

    template<typename T>
        requires (CanConvertConcept<T, Type> && IsVarExprConcept<ExprType> && !IsConstConcept<value_type>)
    void value(T &&value)
    {
        this->updateValue(std::forward<T>(value));
        this->notify();
    }

    void addWeakRef()
    {
        weak_ref_count_++;
    }

    void releaseWeakRef()
    {
        weak_ref_count_--;
        if (weak_ref_count_ == 0)
        {
            ObserverGraph::Instance().removeNode(this->shared_from_this());
        }
    }

private:
    std::atomic<int> weak_ref_count_{0};

};




template<typename ReactType>
class React
{
public:
    explicit React(std::shared_ptr<ReactType> impl = nullptr)
        : impl_(impl)
    {
        if (auto impl = impl_.lock())
        {
            impl->addWeakRef();
        }
    }

    ~React()
    {
        if (auto impl = impl_.lock())
        {
            impl->releaseWeakRef();
        }
    }


    React(const React& other)
        : impl_(other.impl_)
    {
        if (auto impl = impl_.lock())
        {
            impl->addWeakRef();
        }
    }

    React(React&& other) noexcept
        : impl_(std::move(other.impl_))
    {
        other.impl_.reset(); // 显式释放other的引用
    }

    React& operator=(const React& other)
    {
        if (this != &other)
        {
            if (auto impl = impl_.lock())
            {
                impl->releaseWeakRef();
            }
            impl_ = other.impl_;

            if (auto impl = impl_.lock())
            {
                impl->addWeakRef();
            }
        }
        return *this;
    }

    React& operator=(React&& other) noexcept
    {
        if (this != &other)
        {
            if (auto impl = impl_.lock())
            {
                impl->releaseWeakRef();
            }
            impl_ = std::move(other.impl_);
            other.impl_.reset(); // 显式释放other的引用
        }
        return *this;
    }

    explicit operator bool() const
    {
        return !impl_.expired();
    }

    auto get() const
        requires (IsDataReactConcept<ReactType>)
    {
        return getImpl()->get();
    }

        
    void value(auto&& value)
        requires (IsDataReactConcept<ReactType>)
    {
        getImpl()->value(std::forward<decltype(value)>(value));
    }


    std::shared_ptr<ReactType> getImpl() const
    {
        if (auto impl = impl_.lock())
        {
            return impl;
        }
        throw std::runtime_error("react is expired");
    }

private:

    std::weak_ptr<ReactType> impl_;
};





template<typename Type>
auto var(Type&& t)
{
    auto impl_ptr = std::make_shared<ReactImpl<std::decay_t<Type>>>(std::forward<Type>(t));

    ObserverGraph::Instance().addNode(impl_ptr);

    return React(impl_ptr);
}


template<typename Type>
auto constVar(Type&& t)
{
    auto impl_ptr = std::make_shared<ReactImpl<const std::decay_t<Type>>>(std::forward<Type>(t));

    ObserverGraph::Instance().addNode(impl_ptr);

    return React(impl_ptr);
}


template<typename Func, typename... Args>
auto calc(Func&& func, Args&&... args)
{
    auto impl_ptr = std::make_shared<ReactImpl<std::decay_t<Func>, std::decay_t<Args>...>>(std::forward<Func>(func), std::forward<Args>(args)...);

    ObserverGraph::Instance().addNode(impl_ptr);

    return React(impl_ptr);
}


template<typename Func, typename... Args>
auto action(Func&& func, Args&&... args)
{
    return calc(std::forward<Func>(func), std::forward<Args>(args)...);
}


} // namespace reactions