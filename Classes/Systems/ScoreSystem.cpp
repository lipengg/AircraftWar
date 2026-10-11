#include "Systems/ScoreSystem.h"
#include "cocos2d.h"

USING_NS_CC;

namespace aw {

const char* ScoreSystem::kBestKey = "aw_best_score";

ScoreSystem::ScoreSystem()
{
    load();
}

void ScoreSystem::load()
{
    _best = UserDefault::getInstance()->getIntegerForKey(kBestKey, 0);
}

void ScoreSystem::save()
{
    UserDefault::getInstance()->setIntegerForKey(kBestKey, _best);
    UserDefault::getInstance()->flush();
}

void ScoreSystem::add(int value)
{
    _score += value;
    if (_score > _best)
    {
        _best = _score;
        _newBest = true;
        save();
    }
}

void ScoreSystem::reset()
{
    _score = 0;
    _newBest = false;
}

} // namespace aw
