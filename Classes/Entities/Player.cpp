#include "Entities/Player.h"
#include "TextureFactory.h"

#include <algorithm>

USING_NS_CC;

namespace aw {

bool Player::init()
{
    if (!Sprite::initWithTexture(TextureFactory::texture("player")))
    {
        return false;
    }
    resetToStart();
    return true;
}

void Player::resetToStart()
{
    const Balance b = Balance::makeDefault();
    _hp = b.playerHp;
    _shield = 0;
    _power = 1;
    _bombs = b.startBombs;
    _maxPower = b.maxPower;
    _speed = b.playerSpeed;
    _invincibleTime = b.invincibleTime;
    _invincible = 0.0f;
    _alive = true;
    setVisible(true);
    setOpacity(255);
    setScale(1.0f);
    setPosition(clampToPlayArea(Vec2(DESIGN_W * 0.5f, SAFE_BOTTOM + 90.0f), radius()));
}

void Player::updatePlayer(float dt, const InputController::State& input)
{
    if (!_alive)
    {
        return;
    }

    Vec2 pos = getPosition();
    if (input.pointerActive)
    {
        Vec2 target = clampToPlayArea(input.pointerPos + Vec2(0.0f, 76.0f), radius());
        Vec2 delta = target - pos;
        const float dist = delta.length();
        if (dist > 1.0f)
        {
            const float step = std::min(dist, _speed * dt);
            pos += delta / dist * step;
        }
    }
    else if (input.axis.lengthSquared() > 0.0f)
    {
        pos += input.axis * _speed * dt;
    }
    setPosition(clampToPlayArea(pos, radius()));

    if (_invincible > 0.0f)
    {
        _invincible -= dt;
        const int blink = static_cast<int>(_invincible * 14.0f);
        setOpacity((blink % 2 == 0) ? 120 : 255);
        if (_invincible <= 0.0f)
        {
            setOpacity(255);
        }
    }
}

void Player::fillBulletSpawns(std::vector<BulletSpawn>& out, const Balance& balance) const
{
    out.clear();
    const Vec2 p = getPosition();

    BulletSpawn center;
    center.pos = p + Vec2(0.0f, 34.0f);
    center.vel = Vec2(0.0f, balance.playerBulletSpeed);
    center.damage = 1;
    center.radius = 6.0f;
    center.friendly = true;
    center.texture = "bullet_player";

    if (_power <= 1)
    {
        out.push_back(center);
        return;
    }

    BulletSpawn left = center;
    left.pos = p + Vec2(-16.0f, 26.0f);
    BulletSpawn right = center;
    right.pos = p + Vec2(16.0f, 26.0f);
    out.push_back(left);
    out.push_back(right);

    if (_power >= 3)
    {
        out.push_back(center);
        BulletSpawn sideL = center;
        sideL.pos = p + Vec2(-26.0f, 18.0f);
        sideL.vel = Vec2(-120.0f, balance.playerBulletSpeed);
        BulletSpawn sideR = center;
        sideR.pos = p + Vec2(26.0f, 18.0f);
        sideR.vel = Vec2(120.0f, balance.playerBulletSpeed);
        out.push_back(sideL);
        out.push_back(sideR);
    }
}

bool Player::takeDamage(int amount)
{
    if (!_alive || _invincible > 0.0f)
    {
        return false;
    }
    if (_shield > 0)
    {
        --_shield;
        _invincible = _invincibleTime * 0.55f;
        return false;
    }

    _hp -= amount;
    _invincible = _invincibleTime;
    if (_hp <= 0)
    {
        _hp = 0;
        _alive = false;
        return true;
    }
    return false;
}

void Player::collect(PickupType type)
{
    switch (type)
    {
        case PickupType::Power:
            _power = std::min(_maxPower, _power + 1);
            break;
        case PickupType::Shield:
            _shield = std::min(3, _shield + 1);
            break;
        case PickupType::Bomb:
            _bombs = std::min(5, _bombs + 1);
            break;
    }
}

bool Player::consumeBomb()
{
    if (!_alive || _bombs <= 0)
    {
        return false;
    }
    --_bombs;
    return true;
}

} // namespace aw
