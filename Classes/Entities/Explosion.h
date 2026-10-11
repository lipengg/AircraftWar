#pragma once

#include "cocos2d.h"

namespace aw {

class Explosion : public cocos2d::Sprite
{
public:
    float timer = 0.0f;
    float duration = 0.28f;
    bool alive = false;

    CREATE_FUNC(Explosion);
    bool init() override;
    void resetState();
};

} // namespace aw
