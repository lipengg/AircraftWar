#include "Layers/PauseLayer.h"
#include "GameConfig.h"

USING_NS_CC;

namespace aw {

bool PauseLayer::init()
{
    if (!LayerColor::initWithColor(Color4B(0, 0, 0, 160)))
    {
        return false;
    }

    auto label = Label::createWithSystemFont("Paused", "Arial", 48);
    label->setPosition(Vec2(DESIGN_W * 0.5f, DESIGN_H * 0.62f));
    addChild(label, 1);

    auto resume = MenuItemLabel::create(
        Label::createWithSystemFont("Resume", "Arial", 30),
        [](Ref*) {
            EventCustom ev("aw_toggle_pause");
            Director::getInstance()->getEventDispatcher()->dispatchEvent(&ev);
        });
    resume->setPosition(Vec2(DESIGN_W * 0.5f, DESIGN_H * 0.48f));

    auto quit = MenuItemLabel::create(
        Label::createWithSystemFont("Quit to Menu", "Arial", 26),
        [](Ref*) {
            EventCustom ev("aw_quit_to_menu");
            Director::getInstance()->getEventDispatcher()->dispatchEvent(&ev);
        });
    quit->setPosition(Vec2(DESIGN_W * 0.5f, DESIGN_H * 0.36f));

    auto menu = Menu::create(resume, quit, nullptr);
    menu->setPosition(Vec2::ZERO);
    addChild(menu, 1);

    return true;
}

} // namespace aw
