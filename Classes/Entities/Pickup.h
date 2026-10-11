#pragma once

#include "cocos2d.h"
#include "GameConfig.h"

namespace aw {

class Pickup : public cocos2d::Sprite
{
public:
    PickupType type = PickupType::Power;
    float speed = 220.0f;
    bool alive = false;

    CREATE_FUNC(Pickup);
    bool init() override;
    void spawn(PickupType t, const cocos2d::Vec2& pos);
};

} // namespace aw
