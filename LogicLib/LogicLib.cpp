#include "LogicLib.hpp"
#include <dlfcn.h>
#include <iostream>

int sampleSum(int a, int b)
{
	return a + b;
}

LogicUnit::LogicUnit() : libraries_(initLibraries()),
	windows_(initWindows()),
		gameState_(height, std::vector<size_t>(width, 0))
{
	gameState_[headPos_.y][headPos_.x] = head;
}

void LogicUnit::loopTheGame()
{
	currentLibraryIndex_ = dummy;
	windows_[currentLibraryIndex_]->openWindow(width, height);
	windows_[currentLibraryIndex_]->drow(gameState_);
	while (true){
		const auto response =
			windows_[currentLibraryIndex_]->getResponse();
		updateGameState(response);
	}
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

void LogicUnit::updateGameState(responseType response)
{
	switch(response){
		case noResponse:
			windows_[currentLibraryIndex_]->drow(gameState_);
			break;
		case endGame:
			windows_[currentLibraryIndex_]->closeWindow();
			exit(0);
		case left: 
			headPos_.x = (headPos_.x == 0) ? 0 : headPos_.x - 1;
			gameState_[headPos_.y][headPos_.x] = head;
			windows_[currentLibraryIndex_]->drow(gameState_);
			break;
		case right:
			headPos_.x = (headPos_.x + 1) % gameState_[0].size();
			gameState_[headPos_.y][headPos_.x] = head;
			windows_[currentLibraryIndex_]->drow(gameState_);
			break;
		case up:
			headPos_.y = (headPos_.y == 0) ? 0 : headPos_.y - 1;
			gameState_[headPos_.y][headPos_.x] = head;
			windows_[currentLibraryIndex_]->drow(gameState_);
			break;
		case down:
			headPos_.y = (headPos_.y + 1) % gameState_.size();
			gameState_[headPos_.y][headPos_.x] = head;
			windows_[currentLibraryIndex_]->drow(gameState_);
			break;
		case toNcurses:
			windows_[currentLibraryIndex_]->closeWindow();
			currentLibraryIndex_ = ncurses;
			windows_[currentLibraryIndex_]->openWindow(width, height);
			windows_[currentLibraryIndex_]->drow(gameState_);
			break;
		case toDummy:
			windows_[currentLibraryIndex_]->closeWindow();
			currentLibraryIndex_ = dummy;
			windows_[currentLibraryIndex_]->openWindow(width, height);
			windows_[currentLibraryIndex_]->drow(gameState_);
	}
}

LogicUnit::~LogicUnit()
{
	for (auto& library : libraries_)
		dlclose(library);
}
