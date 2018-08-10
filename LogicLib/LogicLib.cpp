#include "LogicLib.hpp"
#include <dlfcn.h>
#include <iostream>

int sampleSum(int a, int b)
{
	return a + b;
}

LogicUnit::LogicUnit() : libraries_(initLibraries()),
	windows_(initWindows()),
		gameState_(height, std::vector<size_t>(width, 0)) {}

void LogicUnit::loopTheGame()
{
	windows_[currentLibraryIndex_]->openWindow(width, height);
	windows_[currentLibraryIndex_]->drow(gameState_);
	windows_[currentLibraryIndex_]->closeWindow();
	
	currentLibraryIndex_ = dummy;
	windows_[currentLibraryIndex_]->drow(gameState_);
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

auto LogicUnit::initWindows()
	-> std::vector<windowPtr>
{
	std::vector<windowPtr> result;
	const auto factoryFunctionName = "create";
	using factoryFunctionType = IWindow* (*)();
	for (size_t i = 0; i < nbLibraries; ++i){
		auto factoryFunctionPtr =
			reinterpret_cast<factoryFunctionType>(dlsym(libraries_[i],
				factoryFunctionName));
		result.push_back(windowPtr{factoryFunctionPtr()});
	}
	return result;
}

LogicUnit::~LogicUnit()
{
	for (auto& library : libraries_)
		dlclose(library);
}
