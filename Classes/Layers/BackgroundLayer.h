#pragma once

#include "cocos2d.h"

#include <vector>

namespace aw {

class BackgroundLayer : public cocos2d::Layer
{
public:
    CREATE_FUNC(BackgroundLayer);
    bool init() override;
    void update(float dt) override;
    void pauseScroll();
    void resumeScroll();

private:
    struct Star
    {
        cocos2d::Sprite* sprite = nullptr;
        float speed = 0.0f;
    };

    std::vector<Star> _stars;
    bool _paused = false;
};

} // namespace aw
