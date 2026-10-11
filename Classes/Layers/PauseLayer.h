#pragma once

#include "cocos2d.h"

namespace aw {

class PauseLayer : public cocos2d::LayerColor
{
public:
    CREATE_FUNC(PauseLayer);
    bool init() override;
};

} // namespace aw
