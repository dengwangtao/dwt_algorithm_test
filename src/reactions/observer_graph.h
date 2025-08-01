#pragma once

#include <memory>
#include <unordered_set>


#include "reactions/observer_node.h"
#include "reactions/singleton.h"


namespace reactions
{



class ObserverGraph : public Singleton<ObserverGraph>
{
public:
    using NodePtr = std::shared_ptr<ObserverNode>;

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



}