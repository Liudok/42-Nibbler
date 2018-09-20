#include "LogicLib/ArgcArgvManager.hpp"
#include "LogicLib/LogicUnit.hpp"
#include <iostream>

int main(int ac, const char** av)
{
    try{
        const auto parameters = ArgcArgvManager::parseParameters(ac, av);
        while (LogicUnit(parameters).loopTheGame());
    }
    catch (std::exception& e){
        std::cerr << boldRedBegin << "Exception caught in main: " <<
            e.what() << boldRedEnd << std::endl;
        return 1;
    }
    return 0;
}
