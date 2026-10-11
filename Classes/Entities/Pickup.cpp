#include "Entities/Pickup.h"
#include "TextureFactory.h"

USING_NS_CC;

namespace aw {

bool Pickup::init()
{
    if (!Sprite::initWithTexture(TextureFactory::texture("pickup_power")))
    {
        return false;
    }
    alive = false;
    setVisible(false);
    return true;
}

void Pickup::spawn(PickupType t, const Vec2& pos)
{
    type = t;
    alive = true;
    setPosition(pos);
    const char* key = "pickup_power";
    switch (t)
    {
        case PickupType::Power:  key = "pickup_power"; break;
        case PickupType::Shield: key = "pickup_shield"; break;
        case PickupType::Bomb:   key = "pickup_bomb"; break;
    }
    Texture2D* tex = TextureFactory::texture(key);
    setTexture(tex);
    setTextureRect(Rect(0.0f, 0.0f, tex->getContentSize().width, tex->getContentSize().height));
    setVisible(true);
}

} // namespace aw
