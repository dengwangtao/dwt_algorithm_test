#pragma once


#include <vector>
#include <functional>

#include "reactions/concept.h"


namespace reactions
{



class ObserverNode
{
public:
    using NotifyFunc = std::function<void()>;

    // virtual ~ObserverNode() = default;

    // event
    // virtual void valueChanged() {};

    void addObserver(const NotifyFunc& f)
    {
        observers_.emplace_back(f);
    }


    // 订阅agrs的每个对象，并将this添加到其观察者订阅列表中
    template<typename... Args>
    void subscribe(const NotifyFunc& f, Args&&... args)
    {
        // 左折叠
        (void)(..., args.addObserver(f));

        // or (c++17)支持
        // std::initializer_list<int> il = { (args.addObserver(this), 0)... };
    }

    void notify()
    {
        for (const auto& observer : observers_)
        {
            // observer->valueChanged();
            observer();
        }
    }


private:
    // std::vector<ObserverNode*> observers_;

    std::vector<NotifyFunc> observers_;
};



}