#include "LogicLib.hpp"
#include <dlfcn.h>
#include <iostream>

int sampleSum(int a, int b)
{
	return a + b;
}

LogicUnit::LogicUnit() : libraries_(initLibraries()),
	librariesFunctionsGetResponsePtrs_(initGetResponseFunctions()),
		librariesFunctionsDrowPtrs_(initDrowFunctions()),
			currentLibrary_(ncurses),
				gameState_(height, std::vector<size_t>(width, 0)) {}

void LogicUnit::loopTheGame()
{
	drow();
	getResponse();
	currentLibrary_ = dummy;
	drow();
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

auto LogicUnit::initDrowFunctions()
	-> std::array<drowFunctionsType, nbLibraries>
{
	std::array<drowFunctionsType, nbLibraries>
		librariesFunctionsDrowPtrs;
	const auto getResponseFunctionName = "drow";
	for (size_t currentLib = 0; currentLib < nbLibraries; ++currentLib){
		librariesFunctionsDrowPtrs[currentLib] =
			reinterpret_cast<drowFunctionsType>(dlsym(libraries_[currentLib],
				getResponseFunctionName));
		if (!librariesFunctionsDrowPtrs[currentLib])
			throw std::runtime_error("Drow function not found");
	}
	return librariesFunctionsDrowPtrs;
}

auto LogicUnit::getResponse() const
	-> responseType
{
	return static_cast<responseType>
		(librariesFunctionsGetResponsePtrs_[currentLibrary_]());
}

void LogicUnit::drow() const
{
	librariesFunctionsDrowPtrs_[currentLibrary_](gameState_);
}

LogicUnit::~LogicUnit()
{
	for (auto& library : libraries_)
		dlclose(library);
}
