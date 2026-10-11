#pragma once

#include "cocos2d.h"

namespace aw {

class Bullet : public cocos2d::Sprite
{
public:
    bool friendly = true;
    int damage = 1;
    float radius = 6.0f;
    bool alive = false;

    CREATE_FUNC(Bullet);
    bool init() override;
    void launch(const cocos2d::Vec2& pos, const cocos2d::Vec2& vel, bool isFriendly, int dmg, float r);
    void update(float dt) override;
    void setVelocity(const cocos2d::Vec2& v) { _velocity = v; }
    const cocos2d::Vec2& velocity() const { return _velocity; }

private:
    cocos2d::Vec2 _velocity = cocos2d::Vec2::ZERO;
};

} // namespace aw
