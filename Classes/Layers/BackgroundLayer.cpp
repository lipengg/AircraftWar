#include "Layers/BackgroundLayer.h"
#include "GameConfig.h"
#include "TextureFactory.h"

USING_NS_CC;

namespace aw {
namespace {
constexpr int kStarCount = 64;
constexpr float kStarMargin = 12.0f;
}

bool BackgroundLayer::init()
{
    if (!Layer::init())
    {
        return false;
    }

    // Deep-space base color.
    addChild(LayerColor::create(Color4B(10, 12, 26, 255)), -1);

    // Procedural parallax starfield: tiny generated star sprites wrap around
    // the screen vertically, so the field scrolls seamlessly forever.
    for (int i = 0; i < kStarCount; ++i)
    {
        auto star = Sprite::createWithTexture(TextureFactory::texture("star"));
        const float scale = randRange(0.6f, 2.0f);
        star->setScale(scale);
        star->setPosition(Vec2(randRange(0.0f, DESIGN_W), randRange(0.0f, DESIGN_H)));
        star->setOpacity(static_cast<GLubyte>(randRange(70.0f, 230.0f)));
        addChild(star, 0);

        Star s;
        s.sprite = star;
        // Bigger/brighter stars scroll slightly faster (parallax depth).
        s.speed = 50.0f + scale * 70.0f;
        _stars.push_back(s);
    }

    scheduleUpdate();
    return true;
}

void BackgroundLayer::update(float dt)
{
    if (_paused)
    {
        return;
    }

    for (auto& s : _stars)
    {
        Vec2 p = s.sprite->getPosition();
        p.y -= s.speed * dt;
        if (p.y < -kStarMargin)
        {
            p.y = DESIGN_H + kStarMargin;
            p.x = randRange(0.0f, DESIGN_W);
        }
        s.sprite->setPosition(p);
    }
}

void BackgroundLayer::pauseScroll()
{
    _paused = true;
}

void BackgroundLayer::resumeScroll()
{
    _paused = false;
}

} // namespace aw
