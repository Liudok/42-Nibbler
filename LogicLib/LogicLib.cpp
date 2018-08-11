#include "LogicLib.hpp"
#include <dlfcn.h>
#include <iostream>

int sampleSum(int a, int b)
{
	return a + b;
}

LogicUnit::LogicUnit() : libraries_(initLibraries()),
	windows_(initWindows()),
		gameField_(height, std::vector<size_t>(width, 0))
{
	snake_.fillMap(gameField_);
}

void LogicUnit::loopTheGame()
{
	currentLibraryIndex_ = dummy;
	windows_[currentLibraryIndex_]->openWindow(width, height);
	windows_[currentLibraryIndex_]->drow(gameField_);
	while (true)
		updateGameState(windows_[currentLibraryIndex_]->getResponse());
}

auto LogicUnit::initLibraries()
	-> std::array<ptrToLibraryType, nbLibraries>
{
	std::array<ptrToLibraryType, nbLibraries> libraries;
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

void LogicUnit::updateGameState(responseType response)
{
	switch(response){
		case noResponse:
			windows_[currentLibraryIndex_]->drow(gameField_);
			break;
		case endGame:
			windows_[currentLibraryIndex_]->closeWindow();
			exit(0);
		case left:
			if (snake_.getHeadPosition().x == 0){
				windows_[currentLibraryIndex_]->closeWindow();
				exit(0);
			}
			snake_.move(left);
			snake_.fillMap(gameField_);
			windows_[currentLibraryIndex_]->drow(gameField_);
			break;
		case right:
			if (snake_.getHeadPosition().x == gameField_[0].size() - 1){
				windows_[currentLibraryIndex_]->closeWindow();
				exit(0);
			}
			snake_.move(right);
			snake_.fillMap(gameField_);
			windows_[currentLibraryIndex_]->drow(gameField_);
			break;
		case up:
			if (snake_.getHeadPosition().y == 0){
				windows_[currentLibraryIndex_]->closeWindow();
				exit(0);
			}
			snake_.move(up);
			snake_.fillMap(gameField_);
			windows_[currentLibraryIndex_]->drow(gameField_);
			break;
		case down:
			if (snake_.getHeadPosition().y == gameField_.size() - 1){
				windows_[currentLibraryIndex_]->closeWindow();
				exit(0);
			}
			snake_.move(down);
			snake_.fillMap(gameField_);
			windows_[currentLibraryIndex_]->drow(gameField_);
			break;
		case toNcurses:
			windows_[currentLibraryIndex_]->closeWindow();
			currentLibraryIndex_ = ncurses;
			windows_[currentLibraryIndex_]->openWindow(width, height);
			windows_[currentLibraryIndex_]->drow(gameField_);
			break;
		case toDummy:
			windows_[currentLibraryIndex_]->closeWindow();
			currentLibraryIndex_ = dummy;
			windows_[currentLibraryIndex_]->openWindow(width, height);
			windows_[currentLibraryIndex_]->drow(gameField_);
	}
}

LogicUnit::~LogicUnit()
{
	for (auto& library : libraries_)
		dlclose(library);
}
