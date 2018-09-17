#include "ArgcArgvManager.hpp"
#include <regex>

auto ArgcArgvManager::parseParameters(int argc, const char** argv)
    -> NibblerParameters
{
    std::vector<std::string> strings;
    for (int i = 1; i < argc; ++i)
        strings.push_back(argv[i]);
    const auto windowSize = defineWindowSize(strings);
    const auto gameMode = defineGameMode(strings);
    return {windowSize.first, windowSize.second, gameMode};
}

auto ArgcArgvManager::defineWindowSize(std::vector<std::string> const& strings)
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

GameMode ArgcArgvManager::defineGameMode(std::vector<std::string> const& strings)
{
    std::string gameModes[nbGameModes] =
        { "classic", "granny", "insane", "rasta" };
    for (const auto& str : strings)
        for (size_t i = 0; i < nbGameModes; ++i)
            if (gameModes[i] == str)
                return static_cast<GameMode>(i);
    return classic;
}

bool ArgcArgvManager::validNumber(std::string const& str)
{
    const auto number = std::stoul(str);
    return number > 15 && number < 150;
}

std::string ArgcArgvManager::readOutputOfCommand(std::string const& cmnd)
{
    const auto BUFF_SIZE = 32;
    std::stringstream ss;
    char buff[BUFF_SIZE];
    FILE* ret = popen(cmnd.c_str(), "r");
    while (!feof(ret))
        if (fgets(buff, BUFF_SIZE, ret))
            ss << buff;
    pclose(ret);
    std::string str(ss.str());
    str.erase(str.end() - 1);
    return str;
}
