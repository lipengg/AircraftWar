#include "Scenes/MainMenuScene.h"

#include "GameConfig.h"
#include "Layers/BackgroundLayer.h"
#include "Scenes/GameScene.h"
#include "Systems/ScoreSystem.h"

USING_NS_CC;

Scene* MainMenuScene::createScene()
{
    return MainMenuScene::create();
}

bool MainMenuScene::init()
{
    if (!Scene::init())
    {
        return false;
    }

    addChild(aw::BackgroundLayer::create(), aw::Z_BACKGROUND);

    auto title = Label::createWithSystemFont("AIRCRAFT WAR", "Arial", 64);
    title->setPosition(Vec2(aw::DESIGN_W * 0.5f, aw::DESIGN_H * 0.72f));
    title->setColor(Color3B(120, 230, 255));
    addChild(title, aw::Z_HUD);

    aw::ScoreSystem score;
    auto best = Label::createWithSystemFont("Best: " + std::to_string(score.best()), "Arial", 28);
    best->setPosition(Vec2(aw::DESIGN_W * 0.5f, aw::DESIGN_H * 0.62f));
    addChild(best, aw::Z_HUD);

    auto hint = Label::createWithSystemFont("Drag / WASD to move, Space to bomb", "Arial", 24);
    hint->setPosition(Vec2(aw::DESIGN_W * 0.5f, aw::DESIGN_H * 0.30f));
    hint->setColor(Color3B(190, 200, 220));
    addChild(hint, aw::Z_HUD);

    auto start = MenuItemLabel::create(Label::createWithSystemFont("START", "Arial", 40), [](Ref*) {
        Director::getInstance()->replaceScene(TransitionFade::create(0.25f, GameScene::createScene()));
    });
    start->setPosition(Vec2(aw::DESIGN_W * 0.5f, aw::DESIGN_H * 0.42f));

    auto menu = Menu::create(start, nullptr);
    menu->setPosition(Vec2::ZERO);
    addChild(menu, aw::Z_HUD);
    return true;
}
