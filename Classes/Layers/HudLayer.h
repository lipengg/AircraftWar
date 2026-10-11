#pragma once

#include "cocos2d.h"

namespace aw {

class HudLayer : public cocos2d::Layer
{
public:
    CREATE_FUNC(HudLayer);
    bool init() override;

    void setScore(int value);
    void setBest(int value);
    void setLives(int value);
    void setPower(int value);
    void setBombs(int value);
    void setPaused(bool v);

private:
    cocos2d::Label* _scoreLabel = nullptr;
    cocos2d::Label* _bestLabel = nullptr;
    cocos2d::Label* _livesLabel = nullptr;
    cocos2d::Label* _powerLabel = nullptr;
    cocos2d::Label* _bombsLabel = nullptr;
    cocos2d::MenuItemImage* _pauseBtn = nullptr;

    int _score = 0;
    int _best = 0;
    int _lives = 0;
    int _power = 0;
    int _bombs = 0;
};

} // namespace aw
