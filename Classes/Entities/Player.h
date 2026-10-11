#pragma once

#include "cocos2d.h"
#include "GameConfig.h"
#include "Systems/InputController.h"

#include <vector>

namespace aw {

class Player : public cocos2d::Sprite
{
public:
    CREATE_FUNC(Player);
    bool init() override;

    void resetToStart();
    void updatePlayer(float dt, const InputController::State& input);
    void fillBulletSpawns(std::vector<BulletSpawn>& out, const Balance& balance) const;

    bool takeDamage(int amount);
    void collect(PickupType type);
    bool consumeBomb();

    bool isAlive() const { return _alive; }
    bool isInvincible() const { return _invincible > 0.0f; }
    int hp() const { return _hp; }
    int shield() const { return _shield; }
    int power() const { return _power; }
    int bombs() const { return _bombs; }
    float radius() const { return 24.0f; }

private:
    int _hp = 3;
    int _shield = 0;
    int _power = 1;
    int _bombs = 1;
    int _maxPower = 3;
    float _speed = 520.0f;
    float _invincible = 0.0f;
    float _invincibleTime = 1.2f;
    bool _alive = true;
};

} // namespace aw
