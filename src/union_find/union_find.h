
#include <unordered_map>


template<typename T, typename Cmp = std::less<T>>
class UnionFind
{
    // 静态断言T为朴素类型
    static_assert(std::is_fundamental<T>::value, "T must be a fundamental type");

public:
    /**
     * 找到value所在的集合的根节点
     * @param value 待查找的元素
     */
    T Find(T value)
    {
        if (nodes_.find(value) == nodes_.end())
        {
            return value;
        }

        if (nodes_[value] != value)
        {
            nodes_[value] = Find(nodes_[value]);
        }
        return nodes_[value];
    }

    void Union(T value1, T value2)
    {
        if (nodes_.find(value1) == nodes_.end())
        {
            nodes_[value1] = value1;
        }
        if (nodes_.find(value2) == nodes_.end())
        {
            nodes_[value2] = value2;
        }

        T root1 = Find(value1);
        T root2 = Find(value2);
        if (root1 != root2)
        {
            if (Cmp()(root1, root2))
            {
                nodes_[root2] = root1;
            }
            else
            {
                nodes_[root1] = root2;
            }
        }
    }

    bool IsConnected(T value1, T value2)
    {
        return Find(value1) == Find(value2);
    }

private:
    std::unordered_map<T, T> nodes_; // {value, parent}
};