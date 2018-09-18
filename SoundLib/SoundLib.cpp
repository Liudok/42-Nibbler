#include "SoundLib.hpp"

IMusicPlayer* create()
{
    return new MusicPlayer;
}

MusicPlayer::MusicPlayer()
{
    initMainTheme();
    initSounds();
}

void MusicPlayer::playMainTheme()
{

    mainTheme.play();
}

void MusicPlayer::pauseMainTheme()
{
    mainTheme.pause();
}

void MusicPlayer::stopMainTheme()
{
    mainTheme.stop();
}

void MusicPlayer::playSound(SoundType i)
{
    sounds[i].play();
}

void MusicPlayer::initMainTheme()
{
    if (!mainTheme.openFromFile("NibblerThirdParties/MainTheme.wav"))
        throw std::runtime_error("MainTheme.wav not found");
}

void MusicPlayer::initSounds()
{
    const char* namesOfSoundFiles[nbSoundTypes] = {
        "NibblerThirdParties/FoodEaten.wav",
        "NibblerThirdParties/FoodEaten.wav",
        "NibblerThirdParties/GameOver.wav"
    };
    for(size_t i = 0; i < nbSoundTypes; ++i){
        if (!soundBuffers[i].loadFromFile(namesOfSoundFiles[i]))
            throw std::runtime_error("Sound not found");
        sounds[i].setBuffer(soundBuffers[i]);
    }
}
