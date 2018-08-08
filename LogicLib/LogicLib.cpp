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
	getResponse();
	currentLibrary_ = dummy;
	getResponse();
}

auto LogicUnit::initLibraries()
	-> std::array<ptrToLibraryType, nbLibraries>
{
	std::array<ptrToLibraryType, LogicUnit::nbLibraries> libraries;
	const char* libraryNames[nbLibraries] =
		{ "libNcursesLib.dylib", "libDummyLib.dylib"};
	for (size_t currentLib = 0; currentLib < nbLibraries; ++currentLib){
		libraries[currentLib] =
			dlopen(libraryNames[currentLib], RTLD_LAZY | RTLD_LOCAL);
		if (!libraries[currentLib])
			throw std::runtime_error("The dl is not found");
	}
	return libraries;
}

auto LogicUnit::initGetResponseFunctions()
	-> std::array<responseFunctionsType, nbLibraries>
{
	std::array<responseFunctionsType, nbLibraries>
		librariesFunctionsGetResponsePtrs;
	const auto getResponseFunctionName = "getResponse";
	for (size_t currentLib = 0; currentLib < nbLibraries; ++currentLib){
		librariesFunctionsGetResponsePtrs[currentLib] =
			reinterpret_cast<responseFunctionsType>(dlsym(libraries_[currentLib],
				getResponseFunctionName));
		if (!librariesFunctionsGetResponsePtrs[currentLib])
			throw std::runtime_error("GetResponse function not found");
	}
	return librariesFunctionsGetResponsePtrs;
}

auto LogicUnit::getResponse()
	-> responseType
{
	return static_cast<responseType>
		(librariesFunctionsGetResponsePtrs_[currentLibrary_]());
}

LogicUnit::~LogicUnit()
{
	for (auto& library : libraries_)
		dlclose(library);
}
