#pragma once

#include <concepts>


// concepts
namespace reactions
{

// 可以转换
template<typename T, typename U>
concept CanConvertConcept = std::is_convertible_v<std::decay_t<T>, std::decay_t<U>>;



struct VarExprTag;
struct CalcExprTag;
// 是否是VarExpr
template<typename ExprType>
concept IsVarExprConcept = std::is_same_v<ExprType, VarExprTag>;




} // namespace reactions


// 类型萃取
namespace reactions
{


// 前向声明
template<typename T, typename... Args>
class ReactImpl;

template<typename ReactType>
class React;



template<typename T>
struct ExpressionTraits
{
    using type = T;
};

// 偏特化
template<typename T>
struct ExpressionTraits<React<ReactImpl<T>>>
{
    using type = T;
};

template<typename Func, typename... Args>
struct ExpressionTraits<React<ReactImpl<Func, Args ...>>>
{
    using type = std::invoke_result_t<Func, typename ExpressionTraits<Args>::type...>;
};

template<typename T, typename... Args>
using ReturnType = typename ExpressionTraits<React<ReactImpl<T, Args...>>>::type;



} // namespace reactions