#include <iostream>
#include "LogicLib/LogicLib.hpp"
#include <dlfcn.h>

int main()
{
	const auto ncursesLibHandle =
		dlopen("libNcursesLib.dylib", RTLD_LAZY | RTLD_LOCAL);
	std::cout << "Library " << (ncursesLibHandle ?
		"found :)" : "not found :(") << std::endl;
	using sampleFuntionType = void(*)();
	const auto sampleFunctionPtr =
		reinterpret_cast<sampleFuntionType>(dlsym(ncursesLibHandle,
			"sampleFunction"));
	sampleFunctionPtr();
	std::cout << "Compilation done!: " <<
		sampleSum(2, 2) << std::endl;
	dlclose(ncursesLibHandle);
}
