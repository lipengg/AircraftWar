#pragma once

#include "cocos2d.h"

namespace aw {

class GameOverLayer : public cocos2d::LayerColor
{
public:
    CREATE_FUNC(GameOverLayer);
    bool init() override;

    void show(int score, int best, bool newBest);

private:
    cocos2d::Label* _label = nullptr;
    cocos2d::Label* _scoreLabel = nullptr;
    cocos2d::Label* _newBestLabel = nullptr;
};

} // namespace aw
