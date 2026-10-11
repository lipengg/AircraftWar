#include "Systems/AudioManager.h"
#include "SimpleAudioEngine.h"
#include "cocos2d.h"

USING_NS_CC;
using namespace CocosDenshion;

namespace aw {

AudioManager& AudioManager::getInstance()
{
    static AudioManager instance;
    return instance;
}

bool AudioManager::init()
{
    const bool hasAudio = FileUtils::getInstance()->isDirectoryExist("audio");
    _musicEnabled = hasAudio;
    _sfxEnabled = hasAudio;
    return true;
}

void AudioManager::playBGM()
{
    if (!_musicEnabled) return;
    SimpleAudioEngine::getInstance()->playBackgroundMusic("audio/bgm.mp3", true);
}

void AudioManager::stopBGM()
{
    SimpleAudioEngine::getInstance()->stopBackgroundMusic(true);
}

void AudioManager::playShoot()
{
    if (!_sfxEnabled) return;
    SimpleAudioEngine::getInstance()->playEffect("audio/shoot.mp3");
}

void AudioManager::playExplosion()
{
    if (!_sfxEnabled) return;
    SimpleAudioEngine::getInstance()->playEffect("audio/explosion.mp3");
}

void AudioManager::playPickup()
{
    if (!_sfxEnabled) return;
    SimpleAudioEngine::getInstance()->playEffect("audio/pickup.mp3");
}

void AudioManager::playBomb()
{
    if (!_sfxEnabled) return;
    SimpleAudioEngine::getInstance()->playEffect("audio/bomb.mp3");
}

} // namespace aw
