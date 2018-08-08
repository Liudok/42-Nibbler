#include "NcursesLib.hpp"
#include <iostream>

extern "C" responseType getResponse()
{
	std::cout << "getResponse of the ncurses dll called" << std::endl;
	return noResponse;
}
