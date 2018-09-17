#include "LogicLib/LogicUnit.hpp"
#include <iostream>
#include <string>
#include <sstream>
#include <stdio.h>

bool validWindowSize(size_t w, size_t h);
std::string readOutputOfCommand(std::string const& cmnd);

struct S
{
    const size_t width;
    const size_t height;
};

int main(int ac, char **av)
{
    if (ac != 3)
    {
        std::cerr << "./Build/Nibbler [width] [height]" << std::endl;
        return 1;
    }
    try{
        const auto width = std::stoul(av[1]);
        const auto height = std::stoul(av[2]);
        if (!validWindowSize(width, height))
        {
            std::cerr << "Invalid size of the field" << std::endl;
            return 2;
        }
        while (LogicUnit(width, height).loopTheGame());
    }
    catch (std::exception& e){
        std::cerr << "ERROR: " << e.what() << std::endl;
        return 3;
    }
    return 0;
}

bool validWindowSize(size_t w, size_t h)
{
    return w > 10 && h > 10 && w < 180 && h < 86;
}

std::string readOutputOfCommand(std::string const& cmnd)
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
