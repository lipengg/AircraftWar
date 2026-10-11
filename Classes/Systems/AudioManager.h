#pragma once

#include <string>

namespace aw {

class AudioManager
{
public:
    static AudioManager& getInstance();

    bool init();
    void setMusicEnabled(bool v) { _musicEnabled = v; }
    void setSfxEnabled(bool v)   { _sfxEnabled = v; }

    void playBGM();
    void stopBGM();
    void playShoot();
    void playExplosion();
    void playPickup();
    void playBomb();

private:
    AudioManager() = default;
    bool _musicEnabled = true;
    bool _sfxEnabled = true;
    unsigned int _bgmId = 0;
};

} // namespace aw
