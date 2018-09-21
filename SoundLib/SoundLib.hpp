#pragma once

#include <Nibbler.hpp>
#include <SFML/Audio.hpp>

extern "C"
{
    IMusicPlayer* create();
}

class MusicPlayer : public IMusicPlayer
{
    public:

    	MusicPlayer();
        MusicPlayer(MusicPlayer const&) = delete;
        MusicPlayer& operator=(MusicPlayer const&) = delete;
        ~MusicPlayer() = default;

        void playMainTheme() override;
        void pauseMainTheme() override;
        void stopMainTheme() override;
        void playSound(SoundType) override;

    private:

        void initMainTheme();
        void initSounds();

    	sf::Music mainTheme;
        sf::SoundBuffer soundBuffers[nbSoundTypes];
        sf::Sound sounds[nbSoundTypes];

};
