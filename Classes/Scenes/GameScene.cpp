#include "Scenes/GameScene.h"
#include "Scenes/MainMenuScene.h"
#include "Systems/AudioManager.h"
#include "TextureFactory.h"

#include <algorithm>
#include <cmath>

USING_NS_CC;

Scene* GameScene::createScene()
{
    return GameScene::create();
}

bool GameScene::init()
{
    if (!Scene::init())
    {
        return false;
    }

    _balance = aw::Balance::makeDefault();
    _spawner = std::unique_ptr<aw::SpawnDirector>(new aw::SpawnDirector(_balance));
    _score.reset();

    _background = aw::BackgroundLayer::create();
    addChild(_background, aw::Z_BACKGROUND);

    _entityLayer = Layer::create();
    addChild(_entityLayer, aw::Z_ENTITY);

    _hud = aw::HudLayer::create();
    addChild(_hud, aw::Z_HUD);

    _pauseLayer = aw::PauseLayer::create();
    _pauseLayer->setVisible(false);
    addChild(_pauseLayer, aw::Z_POPUP);

    _gameOverLayer = aw::GameOverLayer::create();
    _gameOverLayer->setVisible(false);
    addChild(_gameOverLayer, aw::Z_POPUP);

    _input = aw::InputController::create();
    addChild(_input);

    _player = aw::Player::create();
    _player->resetToStart();
    _entityLayer->addChild(_player, aw::Z_PLAYER);

    // Pools
    _playerShotPool = std::unique_ptr<aw::ObjectPool<aw::Bullet>>(new aw::ObjectPool<aw::Bullet>(80,
        [](){ return aw::Bullet::create(); },
        [](aw::Bullet* b){ b->setVisible(false); b->stopAllActions(); b->alive = false; }));

    _enemyShotPool = std::unique_ptr<aw::ObjectPool<aw::Bullet>>(new aw::ObjectPool<aw::Bullet>(60,
        [](){ return aw::Bullet::create(); },
        [](aw::Bullet* b){ b->setVisible(false); b->stopAllActions(); b->alive = false; }));

    _enemyPool = std::unique_ptr<aw::ObjectPool<aw::Enemy>>(new aw::ObjectPool<aw::Enemy>(40,
        [](){ return aw::Enemy::create(); },
        [](aw::Enemy* e){ e->resetState(); }));

    _pickupPool = std::unique_ptr<aw::ObjectPool<aw::Pickup>>(new aw::ObjectPool<aw::Pickup>(24,
        [](){ return aw::Pickup::create(); },
        [](aw::Pickup* p){ p->alive = false; p->setVisible(false); p->stopAllActions(); }));

    _explosionPool = std::unique_ptr<aw::ObjectPool<aw::Explosion>>(new aw::ObjectPool<aw::Explosion>(32,
        [](){ return aw::Explosion::create(); },
        [](aw::Explosion* e){ e->resetState(); }));

    registerEvents();
    scheduleUpdate();
    aw::AudioManager::getInstance().playBGM();
    return true;
}

void GameScene::registerEvents()
{
    _eventDispatcher->addCustomEventListener("aw_toggle_pause", [this](EventCustom*) { togglePause(); });
    _eventDispatcher->addCustomEventListener("aw_restart_game", [this](EventCustom*) { restartGame(); });
    _eventDispatcher->addCustomEventListener("aw_quit_to_menu", [this](EventCustom*) { quitToMenu(); });
}

void GameScene::togglePause()
{
    if (_state == GameState::Running)
    {
        pauseGame();
    }
    else if (_state == GameState::Paused)
    {
        resumeGame();
    }
}

void GameScene::pauseGame()
{
    _state = GameState::Paused;
    _background->pauseScroll();
    _pauseLayer->setVisible(true);
    Director::getInstance()->pause();
}

void GameScene::resumeGame()
{
    Director::getInstance()->resume();
    _pauseLayer->setVisible(false);
    _background->resumeScroll();
    _state = GameState::Running;
}

void GameScene::restartGame()
{
    if (_state == GameState::GameOver)
    {
        _gameOverLayer->setVisible(false);
    }
    else if (_state == GameState::Paused)
    {
        _pauseLayer->setVisible(false);
        Director::getInstance()->resume();
    }

    _state = GameState::Running;
    _score.reset();
    _fireTimer = 0.0f;
    _spawnTimer = _balance.spawnStart;
    _elapsed = 0.0f;

    for (auto b : _playerShots) _playerShotPool->release(b);
    for (auto b : _enemyShots)  _enemyShotPool->release(b);
    for (auto e : _enemies)     _enemyPool->release(e);
    for (auto p : _pickups)     _pickupPool->release(p);
    for (auto x : _explosions)  _explosionPool->release(x);
    _playerShots.clear(); _enemyShots.clear(); _enemies.clear(); _pickups.clear(); _explosions.clear();

    _player->resetToStart();
    _spawner = std::unique_ptr<aw::SpawnDirector>(new aw::SpawnDirector(_balance));
    _background->resumeScroll();
    refreshHud();
}

void GameScene::quitToMenu()
{
    if (_state == GameState::Paused)
    {
        Director::getInstance()->resume();
    }
    Director::getInstance()->replaceScene(TransitionFade::create(0.25f, MainMenuScene::createScene()));
}

void GameScene::gameOver()
{
    _state = GameState::GameOver;
    _background->pauseScroll();
    _gameOverLayer->show(_score.score(), _score.best(), _score.isNewBest());
    _gameOverLayer->setVisible(true);
}

void GameScene::update(float dt)
{
    if (_state != GameState::Running)
    {
        return;
    }

    _elapsed += dt;
    _spawner->update(dt);

    auto input = _input->poll();

    if (input.bombPressed)
    {
        useBomb();
    }

    _player->updatePlayer(dt, input);
    handleFire(dt);
    spawnEnemies(dt);
    updateEnemies(dt);
    updateBullets(dt);
    updatePickups(dt);
    updateExplosions(dt);
    handleCollisions();

    refreshHud();
}

void GameScene::handleFire(float dt)
{
    _fireTimer -= dt;
    if (_fireTimer <= 0.0f)
    {
        _fireTimer = _balance.playerFireInterval;
        std::vector<aw::BulletSpawn> spawns;
        _player->fillBulletSpawns(spawns, _balance);
        for (const auto& s : spawns)
        {
            spawnBullet(s);
        }
    }
}

void GameScene::spawnEnemies(float dt)
{
    _spawnTimer -= dt;
    if (_spawnTimer <= 0.0f)
    {
        spawnEnemy();
        _spawnTimer = _spawner->getSpawnInterval();
    }
}

void GameScene::spawnBullet(const aw::BulletSpawn& spawn)
{
    aw::Bullet* b = spawn.friendly ? _playerShotPool->acquire() : _enemyShotPool->acquire();
    b->launch(spawn.pos, spawn.vel, spawn.friendly, spawn.damage, spawn.radius);
    if (!b->getParent())
    {
        _entityLayer->addChild(b, aw::Z_ENTITY);
    }
    b->setVisible(true);
    if (spawn.friendly)
    {
        _playerShots.pushBack(b);
    }
    else
    {
        _enemyShots.pushBack(b);
    }
}

void GameScene::spawnEnemy()
{
    auto e = _enemyPool->acquire();
    const auto& spec = pickEnemySpec();
    configureEnemy(e, spec);
    if (!e->getParent())
    {
        _entityLayer->addChild(e, aw::Z_ENTITY);
    }
    e->setVisible(true);
    _enemies.pushBack(e);
}

void GameScene::spawnExplosion(const Vec2& pos, float scale)
{
    auto ex = _explosionPool->acquire();
    Texture2D* tex = aw::TextureFactory::texture("explosion");
    ex->setTexture(tex);
    ex->setTextureRect(Rect(0.0f, 0.0f, tex->getContentSize().width, tex->getContentSize().height));
    ex->resetState();
    ex->alive = true;
    ex->setPosition(pos);
    ex->setScale(0.6f * scale);
    ex->setVisible(true);
    if (!ex->getParent())
    {
        _entityLayer->addChild(ex, aw::Z_EFFECT);
    }
    ex->runAction(Sequence::create(
        ScaleTo::create(ex->duration, 1.1f * scale),
        FadeOut::create(0.08f),
        CallFunc::create([ex]() {
            ex->alive = false;
            ex->setVisible(false);
        }),
        nullptr));
    _explosions.pushBack(ex);
}

void GameScene::maybeDropPickup(const Vec2& pos)
{
    float r = aw::rand01();
    if (r < 0.07f)
    {
        spawnPickup(aw::PickupType::Power, pos);
    }
    else if (r < 0.10f)
    {
        spawnPickup(aw::PickupType::Bomb, pos);
    }
    else if (r < 0.13f)
    {
        spawnPickup(aw::PickupType::Shield, pos);
    }
}

void GameScene::spawnPickup(aw::PickupType type, const Vec2& pos)
{
    auto p = _pickupPool->acquire();
    p->spawn(type, pos);
    if (!p->getParent())
    {
        _entityLayer->addChild(p, aw::Z_ENTITY);
    }
    _pickups.pushBack(p);
}

void GameScene::updateEnemies(float dt)
{
    for (int i = static_cast<int>(_enemies.size()) - 1; i >= 0; --i)
    {
        auto e = _enemies.at(i);
        if (!e->alive)
        {
            _enemies.erase(i);
            _enemyPool->release(e);
            continue;
        }

        e->timeAlive += dt;
        Vec2 pos = e->getPosition();
        pos.y -= e->speed * dt;

        if (e->move == aw::MovePattern::Sine)
        {
            pos.x = e->baseX + std::sin(e->timeAlive * e->sineFreq + e->phase) * e->sineAmp;
        }
        else if (e->move == aw::MovePattern::Drift)
        {
            pos.x += std::sin(e->timeAlive * 2.2f + e->phase) * 60.0f * dt;
        }

        pos.x = clampf(pos.x, e->radius, aw::DESIGN_W - e->radius);
        e->setPosition(pos);

        if (e->fireInterval > 0.0f)
        {
            e->fireTimer -= dt;
            if (e->fireTimer <= 0.0f)
            {
                e->fireTimer = e->fireInterval;
                aw::BulletSpawn spawn;
                spawn.pos = pos + Vec2(0.0f, -e->radius - 4.0f);
                spawn.vel = Vec2(0.0f, -e->bulletSpeed);
                spawn.damage = 1;
                spawn.radius = 6.0f;
                spawn.friendly = false;
                spawn.texture = "bullet_enemy";
                spawnBullet(spawn);
            }
        }

        if (pos.y < -e->radius)
        {
            e->alive = false;
        }
    }
}

void GameScene::updateBullets(float dt)
{
    for (int i = static_cast<int>(_playerShots.size()) - 1; i >= 0; --i)
    {
        auto b = _playerShots.at(i);
        b->update(dt);
        if (!b->alive || b->getPositionY() > aw::DESIGN_H + b->radius)
        {
            b->alive = false;
            _playerShots.erase(i);
            _playerShotPool->release(b);
        }
    }
    for (int i = static_cast<int>(_enemyShots.size()) - 1; i >= 0; --i)
    {
        auto b = _enemyShots.at(i);
        b->update(dt);
        if (!b->alive || b->getPositionY() < -b->radius)
        {
            b->alive = false;
            _enemyShots.erase(i);
            _enemyShotPool->release(b);
        }
    }
}

void GameScene::updatePickups(float dt)
{
    for (int i = static_cast<int>(_pickups.size()) - 1; i >= 0; --i)
    {
        auto p = _pickups.at(i);
        if (!p->alive)
        {
            _pickups.erase(i);
            _pickupPool->release(p);
            continue;
        }
        p->setPositionY(p->getPositionY() - p->speed * dt);
        if (p->getPositionY() < -40.0f)
        {
            p->alive = false;
            _pickups.erase(i);
            _pickupPool->release(p);
        }
    }
}

void GameScene::updateExplosions(float dt)
{
    for (int i = static_cast<int>(_explosions.size()) - 1; i >= 0; --i)
    {
        auto ex = _explosions.at(i);
        if (!ex->alive)
        {
            _explosions.erase(i);
            _explosionPool->release(ex);
        }
    }
}

void GameScene::handleCollisions()
{
    if (!_player->isAlive())
    {
        return;
    }

    Vec2 pp = _player->getPosition();
    float pr = _player->radius();

    // Player bullets vs enemies
    for (auto b : _playerShots)
    {
        if (!b->alive) continue;
        for (auto e : _enemies)
        {
            if (!e->alive) continue;
            if (aw::circleHit(b->getPosition(), b->radius, e->getPosition(), e->radius))
            {
                b->alive = false;
                e->hp -= b->damage;
                if (e->hp <= 0)
                {
                    killEnemy(e);
                }
                break;
            }
        }
    }

    // Enemy bullets vs player
    for (auto b : _enemyShots)
    {
        if (!b->alive) continue;
        if (aw::circleHit(b->getPosition(), b->radius, pp, pr * 0.7f))
        {
            b->alive = false;
            damagePlayer(b->damage);
            break;
        }
    }

    // Enemies vs player (body contact)
    for (auto e : _enemies)
    {
        if (!e->alive) continue;
        if (aw::circleHit(e->getPosition(), e->radius * 0.8f, pp, pr * 0.7f))
        {
            killEnemy(e);
            damagePlayer(1);
            break;
        }
    }

    // Pickups vs player
    for (auto p : _pickups)
    {
        if (!p->alive) continue;
        if (aw::circleHit(p->getPosition(), 18.0f, pp, pr * 0.8f))
        {
            p->alive = false;
            _player->collect(p->type);
        }
    }
}

void GameScene::useBomb()
{
    if (!_player->consumeBomb())
    {
        return;
    }

    aw::AudioManager::getInstance().playBomb();

    // Bomb effect: kill all enemies on screen, clear enemy bullets
    for (auto e : _enemies)
    {
        if (e->alive)
        {
            killEnemy(e);
        }
    }
    for (auto b : _enemyShots)
    {
        b->alive = false;
    }
}

void GameScene::damagePlayer(int amount)
{
    if (_player->isInvincible())
    {
        return;
    }
    const bool died = _player->takeDamage(amount);
    aw::AudioManager::getInstance().playExplosion();
    spawnExplosion(_player->getPosition(), 1.2f);
    if (died)
    {
        _player->setVisible(false);
        gameOver();
    }
}

void GameScene::killEnemy(aw::Enemy* enemy)
{
    enemy->alive = false;
    _score.add(enemy->score);
    spawnExplosion(enemy->getPosition(), 1.0f);
    maybeDropPickup(enemy->getPosition());
}

void GameScene::refreshHud()
{
    _hud->setScore(_score.score());
    _hud->setBest(_score.best());
    _hud->setLives(_player->hp());
    _hud->setPower(_player->power());
    _hud->setBombs(_player->bombs());
}

const aw::EnemySpec& GameScene::pickEnemySpec() const
{
    const float t = std::min(1.0f, _elapsed / _balance.rampSeconds);
    if (t < 0.35f)
    {
        return _balance.enemies[0];
    }
    else if (t < 0.75f)
    {
        return aw::rand01() < 0.72f ? _balance.enemies[0] : _balance.enemies[1];
    }
    else
    {
        float r = aw::rand01();
        if (r < 0.55f) return _balance.enemies[0];
        if (r < 0.85f) return _balance.enemies[1];
        return _balance.enemies[2];
    }
}

void GameScene::configureEnemy(aw::Enemy* enemy, const aw::EnemySpec& spec)
{
    Texture2D* tex = aw::TextureFactory::texture(spec.texture);
    enemy->setTexture(tex);
    // Pooled enemies keep the previous texture's quad rect; force a refresh.
    enemy->setTextureRect(Rect(0.0f, 0.0f, tex->getContentSize().width, tex->getContentSize().height));
    enemy->hp = spec.hp;
    enemy->speed = spec.speed;
    enemy->score = spec.score;
    enemy->radius = spec.radius;
    enemy->move = spec.move;
    enemy->sineAmp = spec.sineAmp;
    enemy->sineFreq = spec.sineFreq;
    enemy->fireInterval = spec.fireInterval;
    enemy->bulletSpeed = spec.bulletSpeed;
    enemy->fireTimer = aw::randRange(0.3f, spec.fireInterval > 0.0f ? spec.fireInterval : 0.5f);
    enemy->alive = true;
    enemy->timeAlive = 0.0f;
    enemy->baseX = aw::randRange(spec.radius, aw::DESIGN_W - spec.radius);
    enemy->phase = aw::randRange(0.0f, 6.283f);
    enemy->setPosition(Vec2(enemy->baseX, aw::DESIGN_H + spec.radius));
}
