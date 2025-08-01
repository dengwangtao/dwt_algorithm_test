#pragma once


namespace reactions
{


template<class T>
class Singleton
{
public:
    Singleton(Singleton const&) = delete;
    Singleton& operator=(Singleton const&) = delete;

    static T& Instance()
    {
        static T inst;
        return inst;
    }

protected:
    Singleton() = default;
    ~Singleton() = default;
};


} // namespace reactions