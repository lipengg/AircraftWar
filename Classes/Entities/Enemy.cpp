#include "Entities/Enemy.h"
#include "TextureFactory.h"

USING_NS_CC;

namespace aw {

bool Enemy::init()
{
    // Initialize with a concrete texture so the render quad has real size;
    // configureEnemy() swaps the texture and refreshes the rect on spawn.
    if (!Sprite::initWithTexture(TextureFactory::texture("enemy_grunt")))
    {
        return false;
    }
    resetState();
    return true;
}

void Enemy::resetState()
{
    hp = 1;
    speed = 170.0f;
    score = 10;
    radius = 24.0f;
    move = MovePattern::Straight;
    sineAmp = 0.0f;
    sineFreq = 0.0f;
    fireInterval = 0.0f;
    fireTimer = 0.0f;
    bulletSpeed = 330.0f;
    alive = false;
    timeAlive = 0.0f;
    baseX = 0.0f;
    phase = 0.0f;
    setVisible(false);
}

} // namespace aw
