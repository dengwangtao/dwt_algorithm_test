#pragma once

#include <concepts>

namespace reactions
{


template<typename T>
concept ObserverNodeConcept = requires(T t)
{
    t.valueChanged();
};





// 可以转换
template<typename T, typename U>
concept CanConvertConcept = std::is_convertible_v<std::decay_t<T>, std::decay_t<U>>;



struct VarExprTag;
struct CalcExprTag;
// 是否是VarExpr
template<typename ExprType>
concept IsVarExprConcept = std::is_same_v<ExprType, VarExprTag>;



} // namespace reactions