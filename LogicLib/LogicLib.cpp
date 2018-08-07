#include "LogicLib.hpp"
#include <dlfcn.h>
#include <iostream>

int sampleSum(int a, int b)
{
	return a + b;
}

LogicUnit::LogicUnit() : libraries_(initLibraries()),
	librariesFunctionsGetResponsePtrs_(initGetResponseFunctions()),
		currentLibrary_(ncurses) {}

void LogicUnit::loopTheGame()
{
	while (true){
		const auto response = getResponse();
		if (response == noResponse)
			std::cout << "No response" << std::endl;
		break;
	}
}

auto LogicUnit::initLibraries()
	-> std::array<ptrToLibraryType, nbLibraries>
{
	std::array<ptrToLibraryType, LogicUnit::nbLibraries> libraries;
	const auto ncursesDynamicLibName = "libNcursesLib.dylib";
	libraries[ncurses] =
		dlopen(ncursesDynamicLibName, RTLD_LAZY | RTLD_LOCAL);
	if (!libraries[ncurses])
		throw std::runtime_error("The ncurses dl is not found");
	libraries[dummy] = nullptr;
	return libraries;
}

auto LogicUnit::getResponse()
	-> responseTypes
{
	if (!librariesFunctionsGetResponsePtrs_[currentLibrary_])
		throw std::runtime_error("The getResponse function is not yet"
			" defined for the library");
	return static_cast<responseTypes>
		(librariesFunctionsGetResponsePtrs_[currentLibrary_]());
}

auto LogicUnit::initGetResponseFunctions()
	-> std::array<responseFunctionsType, nbLibraries>
{
	std::array<responseFunctionsType, nbLibraries>
		librariesFunctionsGetResponsePtrs;
	const auto getResponseFunctionName = "sampleFunction";
	librariesFunctionsGetResponsePtrs[ncurses] =
		reinterpret_cast<responseFunctionsType>(dlsym(libraries_[ncurses],
			getResponseFunctionName));
	if (!librariesFunctionsGetResponsePtrs[ncurses])
		throw std::runtime_error("Ncurses getResponse function not found");
	librariesFunctionsGetResponsePtrs[dummy] = nullptr;
	return librariesFunctionsGetResponsePtrs;
}

LogicUnit::~LogicUnit()
{
	for (auto& library : libraries_)
		dlclose(library);
}
