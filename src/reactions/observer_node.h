#pragma once


#include <vector>

#include "reactions/concept.h"


namespace reactions
{



class ObserverNode
{
public:
    virtual ~ObserverNode() = default;

    // event
    virtual void valueChanged() {};

    void addObserver(ObserverNode* observer)
    {
        observers_.push_back(observer);
    }


    // 订阅agrs的每个对象，并将this添加到其观察者订阅列表中
    template<typename... Args>
    void subscribe(Args&&... args)
    {
        // 左折叠
        (void)(..., args.addObserver(this));

        // or (c++17)支持
        // std::initializer_list<int> il = { (args.addObserver(this), 0)... };
    }

    void notify()
    {
        for (const auto& observer : observers_)
        {
            observer->valueChanged();
        }
    }


private:
    std::vector<ObserverNode*> observers_;

};



}