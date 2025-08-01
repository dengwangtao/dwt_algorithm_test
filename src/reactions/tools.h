#pragma once

#include <atomic>
#include <unordered_map>

namespace reactions
{


template<typename T>
T clamp(T v, T min, T max)
{
    return v < min ? min : (v > max ? max : v);
}


class UniqueId
{
public:
    using IdType = unsigned long long;

    UniqueId()
        : id_(generate())
    {
    }


    IdType generate()
    {
        static std::atomic<IdType> nextId{ 1 };
        nextId.fetch_add(1, std::memory_order_relaxed);
        return nextId;
    }

    IdType id() const
    {
        return id_;
    }

    operator IdType() const
    {
        return id_;
    }

    bool operator==(const UniqueId& rhs) const
    {
        return id_ == rhs.id_;
    }

private:
    IdType id_;


    friend struct std::hash<UniqueId>;
};

} // namespace reactions


namespace std
{
template<>
struct hash<reactions::UniqueId>
{
    std::size_t operator()(const reactions::UniqueId& id) const
    {
        return std::hash<reactions::UniqueId::IdType>()(id.id());
    }
};
} // namespace std