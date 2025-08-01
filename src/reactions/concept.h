#pragma once

#include <concepts>

#include "reactions/tools.h"

// concepts
namespace reactions
{

// 可以转换
template<typename T, typename U>
concept CanConvertConcept = std::is_convertible_v<std::decay_t<T>, std::decay_t<U>>;


// 前向声明
struct VarExprTag;
struct CalcExprTag;
// 是否是VarExpr
template<typename ExprType>
concept IsVarExprConcept = std::is_same_v<ExprType, VarExprTag>;



// 是否为const
template<typename T>
concept IsConstConcept = std::is_const_v<T>;


// 是否为void
template<typename T>
concept IsVoidConcept = std::is_void_v<T>;



// 前向声明
class ObserverNode;

// 是否为ReactNode
template<typename T>
concept IsReactNodeConcept = requires(T t)
{
    // {t.get()} -> std::same_as<typename T::value_type>;
    // {t.value(std::declval<typename T::value_type>())};
    // {t.getImpl()} -> std::same_as<std::shared_ptr<T>>;
    {t.shared_from_this()} -> std::same_as<std::shared_ptr<ObserverNode>>;
};



template<typename T>
concept IsDataReactConcept = requires(T t)
{
    typename T::value_type;             // 存在value_type
    requires (IsReactNodeConcept<T>);   // 是ReactNode
    requires (!IsVoidConcept<typename T::value_type>); // value_type不是void
};


class FieldBase;

template<typename T>
concept HasFieldConcept = requires(T t)
{
    requires std::is_base_of_v<FieldBase, std::decay_t<T>>;
    {t.id()} -> std::same_as<UniqueId::IdType>;
};

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