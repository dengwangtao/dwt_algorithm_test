
#include <unordered_map>


template<typename T, typename Cmp = std::less<T>, typename Hasher=std::hash<T>>
class UnionFind
{
public:
    UnionFind(): cmp_(Cmp()), hasher_(Hasher()) {}
    UnionFind(const Cmp& cmp): cmp_(cmp), hasher_(Hasher()) {}
    UnionFind(const Hasher& hasher): cmp_(Cmp()), hasher_(hasher) {}
    UnionFind(const Cmp& cmp, const Hasher& hasher): cmp_(cmp), hasher_(hasher) {}

    /**
     * 找到value所在的集合的根节点
     * @param value 待查找的元素
     */
    const T& Find(const T& value)
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

    void Union(const T& value1, const T& value2)
    {
        if (nodes_.find(value1) == nodes_.end())
        {
            nodes_[value1] = value1;
        }
        if (nodes_.find(value2) == nodes_.end())
        {
            nodes_[value2] = value2;
        }

        const T& root1 = Find(value1);
        const T& root2 = Find(value2);
        if (root1 != root2)
        {
            if (cmp_(root1, root2))
            {
                nodes_[root2] = root1;
            }
            else
            {
                nodes_[root1] = root2;
            }
        }
    }

    bool IsConnected(const T& value1, const T& value2)
    {
        return Find(value1) == Find(value2);
    }

private:
    std::unordered_map<T, T, Hasher> nodes_; // {value, parent}
    Cmp cmp_;
    Hasher hasher_;
};