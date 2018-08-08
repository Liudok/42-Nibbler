#include "DummyLib.hpp"
#include <iostream>

extern "C" responseType getResponse()
{
	std::cout << "getResponse of the dummy dll called" << std::endl;
	return noResponse;
}
