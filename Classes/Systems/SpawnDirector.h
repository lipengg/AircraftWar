#pragma once

#include "cocos2d.h"
#include "GameConfig.h"

namespace aw {

class SpawnDirector
{
public:
    explicit SpawnDirector(const Balance& balance);

    void update(float dt);
    float getSpawnInterval() const { return _currentInterval; }

private:
    const Balance& _b;
    float _timer = 0.0f;
    float _elapsed = 0.0f;
    float _currentInterval = 1.0f;
};

} // namespace aw
