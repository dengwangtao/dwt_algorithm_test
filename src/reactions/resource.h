#pragma once

#include <memory>

#include "reactions/observer_graph.h"

namespace reactions
{

template<typename Type>
class Resource : public ObserverNode
{
public:
    using value_type = Type;

    Resource(): value_(nullptr) {}

    template<typename T>
    Resource(T &&value)
        : value_(std::make_unique<value_type>(std::forward<T>(value)))
    {}

    Resource(const Resource &other) = delete;
    Resource& operator=(const Resource &other) = delete;


    value_type &getValue() const
    {
        if (!value_)
        {
            throw std::runtime_error("Resource has not been initialized");
        }
        return *value_;
    }

    template<typename T>
    void updateValue(T &&value)
    {
        if (!value_)
        {
            value_ = std::make_unique<value_type>(std::forward<T>(value));
        }
        *value_ = std::forward<T>(value);
    }

private:

    std::unique_ptr<value_type> value_;
};


template<>
class Resource<void> : public ObserverNode
{
public:

    Resource() = default;
    Resource(const Resource &other) = delete;
    Resource& operator=(const Resource &other) = delete;
};



} // namespace reactions