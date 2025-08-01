#pragma once

#include "reactions/expression.h"

namespace reactions
{

template<typename Type, typename... Args>
class DataSource : public Expression<Type, Args...>
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
        requires CanConvertConcept<T, Type> && IsVarExprConcept<ExprType>
    void value(T &&value)
    {
        this->updateValue(std::forward<T>(value));
        this->notify();
    }

};



template<typename Type>
auto var(Type&& t)
{
    return DataSource<Type>(std::forward<Type>(t));
}

template<typename Func, typename... Args>
auto calc(Func&& func, Args&&... args)
{
    return DataSource<std::decay_t<Func>, std::decay_t<Args>...>(std::forward<Func>(func), std::forward<Args>(args)...);
}


} // namespace reactions