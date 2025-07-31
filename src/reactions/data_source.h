#pragma once

#include "reactions/expression.h"

namespace reactions
{

template<typename Type, typename... Args>
class DataSource : public Expression<Type, Args...>
{
public:

    using Expression<Type, Args...>::Expression;


    auto get() const
    {
        return this->getValue();
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