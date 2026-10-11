#pragma once

#include "cocos2d.h"

#include "Entities/Bullet.h"
#include "Entities/Enemy.h"
#include "Entities/Explosion.h"
#include "Entities/Pickup.h"
#include "Entities/Player.h"
#include "GameConfig.h"
#include "Layers/BackgroundLayer.h"
#include "Layers/GameOverLayer.h"
#include "Layers/HudLayer.h"
#include "Layers/PauseLayer.h"
#include "Systems/InputController.h"
#include "Systems/ObjectPool.h"
#include "Systems/ScoreSystem.h"
#include "Systems/SpawnDirector.h"

#include <memory>
#include <vector>

class GameScene : public cocos2d::Scene
{
public:
    static cocos2d::Scene* createScene();
    bool init() override;
    void update(float dt) override;
    CREATE_FUNC(GameScene);

private:
    enum class GameState
    {
        Running,
        Paused,
        GameOver
    };

    void registerEvents();
    void togglePause();
    void pauseGame();
    void resumeGame();
    void restartGame();
    void quitToMenu();
    void gameOver();

    void handleFire(float dt);
    void spawnEnemies(float dt);
    void spawnBullet(const aw::BulletSpawn& spawn);
    void spawnEnemy();
    void spawnExplosion(const cocos2d::Vec2& pos, float scale = 1.0f);
    void maybeDropPickup(const cocos2d::Vec2& pos);
    void spawnPickup(aw::PickupType type, const cocos2d::Vec2& pos);

    void updateEnemies(float dt);
    void updateBullets(float dt);
    void updatePickups(float dt);
    void updateExplosions(float dt);
    void handleCollisions();
    void useBomb();
    void damagePlayer(int amount);
    void killEnemy(aw::Enemy* enemy);
    void refreshHud();

    const aw::EnemySpec& pickEnemySpec() const;
    void configureEnemy(aw::Enemy* enemy, const aw::EnemySpec& spec);

private:
    aw::Balance _balance;
    aw::ScoreSystem _score;
    std::unique_ptr<aw::SpawnDirector> _spawner;

    aw::BackgroundLayer* _background = nullptr;
    cocos2d::Layer* _entityLayer = nullptr;
    aw::HudLayer* _hud = nullptr;
    aw::PauseLayer* _pauseLayer = nullptr;
    aw::GameOverLayer* _gameOverLayer = nullptr;
    aw::InputController* _input = nullptr;
    aw::Player* _player = nullptr;

    std::unique_ptr<aw::ObjectPool<aw::Bullet>> _playerShotPool;
    std::unique_ptr<aw::ObjectPool<aw::Bullet>> _enemyShotPool;
    std::unique_ptr<aw::ObjectPool<aw::Enemy>> _enemyPool;
    std::unique_ptr<aw::ObjectPool<aw::Pickup>> _pickupPool;
    std::unique_ptr<aw::ObjectPool<aw::Explosion>> _explosionPool;

    cocos2d::Vector<aw::Bullet*> _playerShots;
    cocos2d::Vector<aw::Bullet*> _enemyShots;
    cocos2d::Vector<aw::Enemy*> _enemies;
    cocos2d::Vector<aw::Pickup*> _pickups;
    cocos2d::Vector<aw::Explosion*> _explosions;

    GameState _state = GameState::Running;
    float _fireTimer = 0.0f;
    float _spawnTimer = 0.35f;
    float _elapsed = 0.0f;
};
