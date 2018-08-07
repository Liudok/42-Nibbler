#include <iostream>
#include "LogicLib/LogicLib.hpp"
#include <dlfcn.h>

int main()
{
	const auto ncursesDynamicLibName = "libNcursesLib.dylib";
	const auto ncursesLibHandle =
		dlopen(ncursesDynamicLibName, RTLD_LAZY | RTLD_LOCAL);
	std::cout << "Library " << (ncursesLibHandle ?
		"found :)" : "not found :(") << std::endl;
	if (!ncursesLibHandle) return 1;
	using sampleFuntionType = void(*)();
	const auto sampleFunctionName = "sampleFunction";
	const auto sampleFunctionPtr =
		reinterpret_cast<sampleFuntionType>(dlsym(ncursesLibHandle,
			sampleFunctionName));
	std::cout << "The sample function from the ncurses dynamic library is " <<
		(sampleFunctionPtr ? "found :)" : "not found") << std::endl;
	if (!sampleFunctionPtr) return 2;
	sampleFunctionPtr();
	std::cout << "Compilation done!: " <<
		sampleSum(2, 2) << std::endl;
	dlclose(ncursesLibHandle);
}
