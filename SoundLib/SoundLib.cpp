#include "SoundLib.hpp"
#include <NibblerUtils.hpp>

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
    std::string path = NibblerUtils::getPathToThirdParties();
    path += "/Audio/MainTheme.wav";
    if (!mainTheme.openFromFile(path.c_str()))
            throw std::runtime_error("MainTheme.wav not found");
}

void MusicPlayer::initSounds()
{
    std::string foodEaten = NibblerUtils::getPathToThirdParties();
    foodEaten += "/Audio/FoodEaten.wav";
    std::string superFoodEaten = NibblerUtils::getPathToThirdParties();
    superFoodEaten += "/Audio/SuperFoodEaten.wav";
    std::string gameOver = NibblerUtils::getPathToThirdParties();
    gameOver += "/Audio/GameOver.wav";
    const char* namesOfSoundFiles[nbSoundTypes] = {
        foodEaten.c_str(),
        superFoodEaten.c_str(),
        gameOver.c_str()
    };
    for(size_t i = 0; i < nbSoundTypes; ++i){
        if (!soundBuffers[i].loadFromFile(namesOfSoundFiles[i]))
            throw std::runtime_error("Sound not found");
        sounds[i].setBuffer(soundBuffers[i]);
    }
}
