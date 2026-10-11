#include "Entities/Explosion.h"
#include "TextureFactory.h"

USING_NS_CC;

namespace aw {

bool Explosion::init()
{
    if (!Sprite::initWithTexture(TextureFactory::texture("explosion")))
    {
        return false;
    }
    resetState();
    return true;
}

void Explosion::resetState()
{
    timer = 0.0f;
    duration = 0.28f;
    alive = false;
    stopAllActions();
    setScale(0.7f);
    setOpacity(255);
    setVisible(false);
}

} // namespace aw
