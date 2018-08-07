#include "NcursesLib.hpp"
#include <iostream>

extern "C" responseTypes sampleFunction()
{
	std::cout << "sampleFunction of the ncurses dll called" << std::endl;
	return noResponse;
}
