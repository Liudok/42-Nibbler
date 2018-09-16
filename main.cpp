#include "LogicLib/LogicLib.hpp"
#include <iostream>

bool validWindowSize(size_t w, size_t h);

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
