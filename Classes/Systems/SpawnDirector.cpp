#include "Systems/SpawnDirector.h"

#include <algorithm>
#include <cmath>

USING_NS_CC;

namespace aw {

SpawnDirector::SpawnDirector(const Balance& balance) : _b(balance)
{
    _currentInterval = _b.spawnStart;
}

void SpawnDirector::update(float dt)
{
    _elapsed += dt;
    const float t = std::min(1.0f, _elapsed / _b.rampSeconds);
    _currentInterval = _b.spawnMin + (_b.spawnStart - _b.spawnMin) * (1.0f - t);
}

} // namespace aw
