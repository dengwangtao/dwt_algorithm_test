#pragma once

#include <atomic>

#include "reactions/expression.h"

namespace reactions
{

template<typename Type, typename... Args>
class ReactImpl : public Expression<Type, Args...>
{
public:
    using SelfType = ReactImpl<Type, Args...>;

    using SuperClass = Expression<Type, Args...>;
    using SuperClass::Expression;
    using value_type = SuperClass::value_type;

    using ExprType = SuperClass::ExprType;


    decltype(auto) get() const
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

    template<typename T>
    SelfType& operator=(T &&value)
    {
        this->value(std::forward<T>(value));
        return *this;
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

            if constexpr (HasFieldConcept<value_type>)
            {
                FieldGraph::Instance().removeField(this->getValue().id());
            }
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

    // auto operator->() const
    // {
    //     return getImpl().get();
    // }

    ReactType& operator*() const
    {
        return *getImpl();
    }

    decltype(auto) get() const
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
using Field = React<ReactImpl<std::decay_t<Type>>>;

class FieldBase
{
public:
    template<typename T>
    auto field(T&& t)
    {
        auto impl_ptr = std::make_shared<ReactImpl<std::decay_t<T>>>(std::forward<T>(t));
        FieldGraph::Instance().addField(id_, impl_ptr->shared_from_this()); // shared_from_this拿到基类的指针
        return React(impl_ptr);
    }

    UniqueId::IdType id() const
    {
        return id_;
    }

private:
    UniqueId id_;
};






template<typename Type>
auto var(Type&& t)
{
    auto impl_ptr = std::make_shared<ReactImpl<std::decay_t<Type>>>(std::forward<Type>(t));

    if constexpr (HasFieldConcept<Type>)
    {
        FieldGraph::Instance().bind(t.id(), impl_ptr->shared_from_this());
    }
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