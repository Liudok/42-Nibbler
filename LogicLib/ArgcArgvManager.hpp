#pragma once

#include <IWindow.hpp>
#include <string>
#include <sstream>
#include <stdio.h>

class ArgcArgvManager
{
    public:

        ArgcArgvManager() = delete;

        static NibblerParameters parseParameters(int argc, const char** argv);

    private:

        static std::pair<size_t, size_t> defineWindowSize(std::vector<std::string> const&);
        static std::pair<size_t, size_t> findOptimalWindowSize();
        static GameMode defineGameMode(std::vector<std::string> const&);
        static bool validNumber(std::string const&);
        static std::string readOutputOfCommand(std::string const& cmnd);

};
