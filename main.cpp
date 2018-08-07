#include <iostream>
#include "LogicLib/LogicLib.hpp"
#include "NcursesLib/NcursesLib.hpp"
#include <dlfcn.h>

int main()
{
	sampleFunction();
	std::cout << "Compilation done!: " <<
		sampleSum(2, 2) << std::endl;
}
