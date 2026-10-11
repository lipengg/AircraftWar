#pragma once

#include "cocos2d.h"

#include <functional>
#include <vector>

namespace aw {

template <typename T>
class ObjectPool
{
public:
    using CreateFunc = std::function<T*()>;
    using ResetFunc = std::function<void(T*)>;

    ObjectPool(size_t prealloc, CreateFunc createFunc, ResetFunc resetFunc)
        : _create(std::move(createFunc))
        , _reset(std::move(resetFunc))
    {
        CCASSERT(_create != nullptr, "ObjectPool requires a create function");
        for (size_t i = 0; i < prealloc; ++i)
        {
            addFree();
        }
    }

    ~ObjectPool()
    {
        for (auto node : _free)
        {
            if (node)
            {
                node->release();
            }
        }
        _free.clear();
    }

    T* acquire()
    {
        if (_free.empty())
        {
            addFree();
        }
        T* node = _free.back();
        _free.pop_back();
        return node;
    }

    void release(T* node)
    {
        if (!node)
        {
            return;
        }
        node->retain();
        node->stopAllActions();
        node->removeFromParent();
        if (_reset)
        {
            _reset(node);
        }
        _free.push_back(node);
    }

private:
    void addFree()
    {
        T* node = _create();
        CCASSERT(node != nullptr, "ObjectPool create function returned null");
        node->retain();
        if (_reset)
        {
            _reset(node);
        }
        _free.push_back(node);
    }

private:
    CreateFunc _create;
    ResetFunc _reset;
    std::vector<T*> _free;
};

} // namespace aw
