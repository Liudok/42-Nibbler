#include "LogicLib.hpp"
#include <dlfcn.h>
#include <iostream>

int sampleSum(int a, int b)
{
	return a + b;
}

LogicUnit::LogicUnit() : libraries_(initLibraries()),
	windows_(initWindows()),
		gameField_(height, std::vector<size_t>(width, 0)),
		snake_(gameField_[0].size(), gameField_.size())
{
	snake_.fillMap(gameField_);
}

void LogicUnit::loopTheGame()
{
	windows_[currentLibraryIndex_]->openWindow(width, height);
	windows_[currentLibraryIndex_]->draw(gameField_);
	std::function<void()> reactFunctions[nbResponses] = {
		std::bind(&LogicUnit::reactToNoResponse, this),
		std::bind(&LogicUnit::reactToLeft, this),
		std::bind(&LogicUnit::reactToRight, this),
		std::bind(&LogicUnit::reactToUp, this),
		std::bind(&LogicUnit::reactToDown, this),
		std::bind(&LogicUnit::reactToToNcurses, this),
		std::bind(&LogicUnit::reactToToDummy, this),
		std::bind(&LogicUnit::reactToEndGame, this) };
	while (!endOfGame_){
		const auto response = windows_[currentLibraryIndex_]->getResponse();
		reactFunctions[response]();
	}
	windows_[currentLibraryIndex_]->closeWindow();
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

void LogicUnit::reactToNoResponse()
{
	snake_.move();
	if (snake_.isOutOfField()){
		endOfGame_ = true;
		return;
	}
	snake_.fillMap(gameField_);
	windows_[currentLibraryIndex_]->draw(gameField_);
}

void LogicUnit::reactToLeft()
{
	snake_.move(left);
	if (snake_.isOutOfField()){
		endOfGame_ = true;
		return;
	}
	snake_.fillMap(gameField_);
	windows_[currentLibraryIndex_]->draw(gameField_);
}

void LogicUnit::reactToRight()
{
	snake_.move(right);
	if (snake_.isOutOfField()){
		endOfGame_ = true;
		return;
	}
	snake_.fillMap(gameField_);
	windows_[currentLibraryIndex_]->draw(gameField_);
}

void LogicUnit::reactToUp()
{
	snake_.move(up);
	if (snake_.isOutOfField()){
		endOfGame_ = true;
		return;
	}
	snake_.fillMap(gameField_);
	windows_[currentLibraryIndex_]->draw(gameField_);
}

void LogicUnit::reactToDown()
{
	snake_.move(down);
	if (snake_.isOutOfField()){
		endOfGame_ = true;
		return;
	}
	snake_.fillMap(gameField_);
	windows_[currentLibraryIndex_]->draw(gameField_);
}

void LogicUnit::reactToToNcurses()
{
	windows_[currentLibraryIndex_]->closeWindow();
	currentLibraryIndex_ = ncurses;
	windows_[currentLibraryIndex_]->openWindow(width, height);
	windows_[currentLibraryIndex_]->draw(gameField_);
}

void LogicUnit::reactToToDummy()
{
	windows_[currentLibraryIndex_]->closeWindow();
	currentLibraryIndex_ = dummy;
	windows_[currentLibraryIndex_]->openWindow(width, height);
	windows_[currentLibraryIndex_]->draw(gameField_);
}

void LogicUnit::reactToEndGame()
{
	windows_[currentLibraryIndex_]->closeWindow();
	endOfGame_ = true;
}

LogicUnit::~LogicUnit()
{
	for (auto& library : libraries_)
		dlclose(library);
}
