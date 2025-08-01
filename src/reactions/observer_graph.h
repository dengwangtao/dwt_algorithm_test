#pragma once

#include <memory>
#include <unordered_set>


#include "reactions/observer_node.h"
#include "reactions/singleton.h"
#include "reactions/tools.h"


namespace reactions
{



using NodePtr = std::shared_ptr<ObserverNode>;
class ObserverGraph : public Singleton<ObserverGraph>
{
public:

    void addNode(NodePtr node)
    {
        nodes_.insert(node);
    }

    void removeNode(NodePtr node)
    {
        nodes_.erase(node);
    }


private:
    std::unordered_set<NodePtr> nodes_;
    
};


class FieldGraph : public Singleton<FieldGraph>
{
    using key_type = UniqueId::IdType;
public:
    void addField(key_type id, NodePtr node)
    {
        field_map_[id].insert(node);
    }

    void removeField(key_type id)
    {
        field_map_.erase(id);
    }

    void bind(key_type id, NodePtr node)
    {
        if (field_map_.count(id) > 0)
        {
            for (auto& fd : field_map_[id])
            {
                fd->addObserver(node.get());
            }
        }
    }

private:
    std::unordered_map<key_type, std::unordered_set<NodePtr>> field_map_;

};



}