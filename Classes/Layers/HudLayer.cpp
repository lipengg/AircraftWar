#include "Layers/HudLayer.h"
#include "GameConfig.h"

USING_NS_CC;

namespace aw {

bool HudLayer::init()
{
    if (!Layer::init())
    {
        return false;
    }

    const float top = DESIGN_H - 18.0f;

    _scoreLabel = Label::createWithSystemFont("Score: 0", "Arial", 26);
    _scoreLabel->setAnchorPoint(Vec2(0, 1));
    _scoreLabel->setPosition(Vec2(16, top));
    addChild(_scoreLabel, Z_HUD);

    _bestLabel = Label::createWithSystemFont("Best: 0", "Arial", 22);
    _bestLabel->setAnchorPoint(Vec2(0, 1));
    _bestLabel->setPosition(Vec2(16, top - 32));
    addChild(_bestLabel, Z_HUD);

    _livesLabel = Label::createWithSystemFont("HP: 3", "Arial", 24);
    _livesLabel->setAnchorPoint(Vec2(1, 1));
    _livesLabel->setPosition(Vec2(DESIGN_W - 16, top));
    addChild(_livesLabel, Z_HUD);

    _powerLabel = Label::createWithSystemFont("P: 1", "Arial", 22);
    _powerLabel->setAnchorPoint(Vec2(1, 1));
    _powerLabel->setPosition(Vec2(DESIGN_W - 16, top - 28));
    addChild(_powerLabel, Z_HUD);

    _bombsLabel = Label::createWithSystemFont("B: 1", "Arial", 22);
    _bombsLabel->setAnchorPoint(Vec2(1, 1));
    _bombsLabel->setPosition(Vec2(DESIGN_W - 16, top - 56));
    addChild(_bombsLabel, Z_HUD);

    auto pauseItem = MenuItemLabel::create(
        Label::createWithSystemFont("||", "Arial", 28),
        [](Ref*) {
            EventCustom ev("aw_toggle_pause");
            Director::getInstance()->getEventDispatcher()->dispatchEvent(&ev);
        });
    pauseItem->setPosition(Vec2(DESIGN_W * 0.5f, top - 18));
    auto menu = Menu::create(pauseItem, nullptr);
    menu->setPosition(Vec2::ZERO);
    addChild(menu, Z_HUD);

    return true;
}

void HudLayer::setScore(int value)
{
    if (_score == value) return;
    _score = value;
    _scoreLabel->setString("Score: " + std::to_string(_score));
}

void HudLayer::setBest(int value)
{
    if (_best == value) return;
    _best = value;
    _bestLabel->setString("Best: " + std::to_string(_best));
}

void HudLayer::setLives(int value)
{
    if (_lives == value) return;
    _lives = value;
    _livesLabel->setString("HP: " + std::to_string(_lives));
}

void HudLayer::setPower(int value)
{
    if (_power == value) return;
    _power = value;
    _powerLabel->setString("P: " + std::to_string(_power));
}

void HudLayer::setBombs(int value)
{
    if (_bombs == value) return;
    _bombs = value;
    _bombsLabel->setString("B: " + std::to_string(_bombs));
}

void HudLayer::setPaused(bool v)
{
    // Visual hint handled by parent visibility.
}

} // namespace aw
