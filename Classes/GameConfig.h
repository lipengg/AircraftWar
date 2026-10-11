#pragma once

#include "cocos2d.h"

#include <cstdlib>
#include <string>
#include <vector>

namespace aw {

constexpr float DESIGN_W = 720.0f;
constexpr float DESIGN_H = 1280.0f;
constexpr float SAFE_BOTTOM = 120.0f;

constexpr int Z_BACKGROUND = 0;
constexpr int Z_ENTITY = 10;
constexpr int Z_PLAYER = 20;
constexpr int Z_EFFECT = 30;
constexpr int Z_HUD = 100;
constexpr int Z_POPUP = 200;

enum class MovePattern
{
    Straight,
    Sine,
    Drift
};

enum class PickupType
{
    Power,
    Shield,
    Bomb
};

enum class BulletSide
{
    Player,
    Enemy
};

struct BulletSpawn
{
    cocos2d::Vec2 pos;
    cocos2d::Vec2 vel;
    int damage = 1;
    float radius = 6.0f;
    bool friendly = true;
    std::string texture;
};

struct EnemySpec
{
    // Pure C++11 aggregate: no default member initializers (NDK r16b clang
    // enforces strict C++11 aggregate rules, unlike MSVC).
    const char* id;
    const char* texture;
    int hp;
    float speed;
    int score;
    float radius;
    MovePattern move;
    float fireInterval;
    float sineAmp;
    float sineFreq;
    float bulletSpeed;
};

struct Balance
{
    int playerHp = 3;
    int startBombs = 1;
    int maxPower = 3;
    float playerSpeed = 520.0f;
    float playerFireInterval = 0.16f;
    float playerBulletSpeed = 920.0f;
    float invincibleTime = 1.2f;
    float spawnStart = 1.05f;
    float spawnMin = 0.34f;
    float rampSeconds = 95.0f;
    std::vector<EnemySpec> enemies;

    static Balance makeDefault()
    {
        Balance b;
        b.enemies = {
            { "grunt", "enemy_grunt", 1, 178.0f, 10, 24.0f, MovePattern::Straight, 0.0f, 0.0f, 0.0f, 0.0f },
            { "weaver", "enemy_weaver", 3, 138.0f, 30, 25.0f, MovePattern::Sine, 1.9f, 96.0f, 3.2f, 330.0f },
            { "drifter", "enemy_drifter", 5, 118.0f, 55, 27.0f, MovePattern::Drift, 1.35f, 0.0f, 0.0f, 360.0f }
        };
        return b;
    }
};

inline float rand01()
{
    return static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX);
}

inline float randRange(float minValue, float maxValue)
{
    return minValue + (maxValue - minValue) * rand01();
}

inline bool circleHit(const cocos2d::Vec2& a, float ra, const cocos2d::Vec2& b, float rb)
{
    const float r = ra + rb;
    return a.distanceSquared(b) <= r * r;
}

inline cocos2d::Vec2 clampToPlayArea(const cocos2d::Vec2& p, float radius)
{
    return cocos2d::Vec2(
        cocos2d::clampf(p.x, radius, DESIGN_W - radius),
        cocos2d::clampf(p.y, radius + 24.0f, DESIGN_H - radius));
}

} // namespace aw
