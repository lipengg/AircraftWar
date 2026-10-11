#pragma once

#include "cocos2d.h"
#include "GameConfig.h"

namespace aw {

class Enemy : public cocos2d::Sprite
{
public:
    int hp = 1;
    float speed = 170.0f;
    int score = 10;
    float radius = 24.0f;
    MovePattern move = MovePattern::Straight;
    float sineAmp = 0.0f;
    float sineFreq = 0.0f;
    float fireInterval = 0.0f;
    float fireTimer = 0.0f;
    float bulletSpeed = 330.0f;
    bool alive = false;
    float timeAlive = 0.0f;
    float baseX = 0.0f;
    float phase = 0.0f;

    CREATE_FUNC(Enemy);
    bool init() override;
    void resetState();
};

} // namespace aw
