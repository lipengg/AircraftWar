#include "Layers/GameOverLayer.h"
#include "GameConfig.h"

USING_NS_CC;

namespace aw {

bool GameOverLayer::init()
{
    if (!LayerColor::initWithColor(Color4B(0, 0, 0, 180)))
    {
        return false;
    }

    _label = Label::createWithSystemFont("GAME OVER", "Arial", 52);
    _label->setPosition(Vec2(DESIGN_W * 0.5f, DESIGN_H * 0.70f));
    addChild(_label, 1);

    _scoreLabel = Label::createWithSystemFont("", "Arial", 30);
    _scoreLabel->setPosition(Vec2(DESIGN_W * 0.5f, DESIGN_H * 0.55f));
    addChild(_scoreLabel, 1);

    _newBestLabel = Label::createWithSystemFont("", "Arial", 26);
    _newBestLabel->setPosition(Vec2(DESIGN_W * 0.5f, DESIGN_H * 0.46f));
    _newBestLabel->setColor(Color3B(255, 220, 60));
    addChild(_newBestLabel, 1);

    auto restart = MenuItemLabel::create(
        Label::createWithSystemFont("Restart", "Arial", 28),
        [](Ref*) {
            EventCustom ev("aw_restart_game");
            Director::getInstance()->getEventDispatcher()->dispatchEvent(&ev);
        });
    restart->setPosition(Vec2(DESIGN_W * 0.5f, DESIGN_H * 0.36f));

    auto menuBtn = MenuItemLabel::create(
        Label::createWithSystemFont("Menu", "Arial", 26),
        [](Ref*) {
            EventCustom ev("aw_quit_to_menu");
            Director::getInstance()->getEventDispatcher()->dispatchEvent(&ev);
        });
    menuBtn->setPosition(Vec2(DESIGN_W * 0.5f, DESIGN_H * 0.26f));

    auto menu = Menu::create(restart, menuBtn, nullptr);
    menu->setPosition(Vec2::ZERO);
    addChild(menu, 1);

    return true;
}

void GameOverLayer::show(int score, int best, bool newBest)
{
    _scoreLabel->setString("Score: " + std::to_string(score) + "  Best: " + std::to_string(best));
    _newBestLabel->setString(newBest ? "New Best!" : "");
    setVisible(true);
}

} // namespace aw
