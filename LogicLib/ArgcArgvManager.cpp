#include "ArgcArgvManager.hpp"
#include <regex>
#include <stdio.h>

auto ArgcArgvManager::parseParameters(int argc, const char** argv)
    -> NibblerParameters
{
    std::vector<std::string> strings;
    for (int i = 1; i < argc; ++i)
        strings.push_back(argv[i]);
    const auto windowSize = defineWindowSize(strings);
    const auto gameMode = defineGameMode(strings);
    const auto libraryType = defineLibraryType(strings);
    const auto musicPlayerState = defineMusicPlayerState(strings);
    return {windowSize.first, windowSize.second,
        gameMode, libraryType, musicPlayerState};
}

auto ArgcArgvManager::defineWindowSize(CmndInput strings)
    -> std::pair<size_t, size_t>
{
    std::regex number("^[0-9]+$");
    std::vector<size_t> numbers;
    for (const auto& str : strings)
        if (std::regex_match(str, number) && validNumber(str))
            numbers.push_back(std::stoul(str));
    if (numbers.size() > 1)
        return {numbers[0], numbers[1]};
    return findOptimalWindowSize();
}

auto ArgcArgvManager::findOptimalWindowSize()
    -> std::pair<size_t, size_t>
{
    const auto width = std::stoul(readOutputOfCommand(
        "system_profiler SPDisplaysDataType | grep Resolution | awk '{print $2}'"));
    const auto height = std::stoul((readOutputOfCommand(
        "system_profiler SPDisplaysDataType | grep Resolution | awk '{print $4}'")));
    const auto sizeFactor = 35;
    return {width / sizeFactor, height / sizeFactor};
}

GameMode ArgcArgvManager::defineGameMode(CmndInput strings)
{
    std::string gameModes[nbGameModes] =
        { "classic", "granny", "insane", "rasta" };
    for (const auto& str : strings)
        for (size_t i = 0; i < nbGameModes; ++i)
            if (gameModes[i] == str)
                return static_cast<GameMode>(i);
    return defaultGameMode;
}

LibraryType ArgcArgvManager::defineLibraryType(CmndInput strings)
{
    std::string libraryTypes[nbLibraries] =
        { "glfw", "sdl", "sfml" };
    for (const auto& str : strings)
        for (size_t i = 0; i < nbLibraries; ++i)
            if (libraryTypes[i] == str)
                return static_cast<LibraryType>(i);
    return defaultLibrary;
}

MusicPlayerState ArgcArgvManager::defineMusicPlayerState(CmndInput strings)
{
    std::string musicPlayerStates[nbMusicPlayerStates] =
        { "on", "soundsOnly", "off" };
    for (const auto& str : strings)
        for (size_t i = 0; i < nbLibraries; ++i)
            if (musicPlayerStates[i] == str)
                return static_cast<MusicPlayerState>(i);
    return defaultMusicPlayerState;
}

bool ArgcArgvManager::validNumber(std::string const& str)
{
    const auto number = std::stoul(str);
    return number > 15 && number < 150;
}

std::string ArgcArgvManager::readOutputOfCommand(std::string const& cmnd)
{
    const auto BUFF_SIZE = 16;
    std::string result;
    char buff[BUFF_SIZE];
    auto ret = popen(cmnd.c_str(), "r");
    while (!feof(ret))
        if (fgets(buff, BUFF_SIZE, ret))
            result += buff;
    pclose(ret);
    if (result.empty()) return result;
    result.erase(result.end() - 1);
    return result;
}
