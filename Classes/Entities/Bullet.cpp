#include "Entities/Bullet.h"
#include "GameConfig.h"
#include "TextureFactory.h"

USING_NS_CC;

namespace aw {

bool Bullet::init()
{
    // Must initialize with a real texture: Sprite::init() leaves the quad
    // rect at ZERO and a later setTexture() does not resize it, which would
    // make pooled bullets render as an invisible zero-size quad.
    if (!Sprite::initWithTexture(TextureFactory::texture("bullet_player")))
    {
        return false;
    }
    alive = false;
    friendly = true;
    damage = 1;
    radius = 6.0f;
    setVisible(false);
    return true;
}

void Bullet::launch(const Vec2& pos, const Vec2& vel, bool isFriendly, int dmg, float r)
{
    Texture2D* tex = TextureFactory::texture(isFriendly ? "bullet_player" : "bullet_enemy");
    setTexture(tex);
    // Refresh geometry/UVs for the new texture (required for pooled sprites).
    setTextureRect(Rect(0.0f, 0.0f, tex->getContentSize().width, tex->getContentSize().height));
    friendly = isFriendly;
    damage = dmg;
    radius = r;
    setPosition(pos);
    alive = true;
    setVisible(true);
    setScale(1.0f);
    setRotation(0.0f);
    setOpacity(255);
    setVelocity(vel);
}

void Bullet::update(float dt)
{
    if (!alive)
    {
        return;
    }
    setPosition(getPosition() + _velocity * dt);
}

} // namespace aw
