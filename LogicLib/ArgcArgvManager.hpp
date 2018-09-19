#pragma once

#include <Nibbler.hpp>
#include <string>

class ArgcArgvManager
{
    public:

        static NibblerParameters parseParameters(int argc, const char** argv);

        ArgcArgvManager() = delete;

    private:

        using CmndInput = std::vector<std::string> const&;

        static std::pair<size_t, size_t> defineWindowSize(CmndInput);
        static std::pair<size_t, size_t> findOptimalWindowSize();
        static GameMode defineGameMode(CmndInput);
        static LibraryType defineLibraryType(CmndInput);
        static MusicPlayerState defineMusicPlayerState(CmndInput);
        static bool validNumber(std::string const&);
        static std::string readOutputOfCommand(std::string const&);

};
