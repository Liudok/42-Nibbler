#pragma once

#include <cstddef>

enum SoundType { foodEaten, superFoodEaten, gameOver };

constexpr size_t nbSoundTypes = 3;

enum MusicPlayerState { on, soundsOnly, off };

constexpr size_t nbMusicPlayerStates = 3;

class IMusicPlayer
{
    public:

        virtual void playMainTheme() = 0;
        virtual void pauseMainTheme() = 0;
        virtual void stopMainTheme() = 0;
        virtual void playSound(SoundType) = 0;
        virtual ~IMusicPlayer() {}

};
