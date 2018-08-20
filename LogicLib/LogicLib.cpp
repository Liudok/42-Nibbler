#include "LogicLib.hpp"
#include <dlfcn.h>
#include <iostream>
#include <chrono>
#include <unistd.h>

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
		std::bind(&LogicUnit::reactToToSDL, this),
		std::bind(&LogicUnit::reactToToSFML, this),
		std::bind(&LogicUnit::reactToEndGame, this),
		std::bind(&LogicUnit::pauseTheGame, this) };
	const auto normalLoopDuration = 700000;
	while (!endOfGame_){
		const auto t0 = std::chrono::high_resolution_clock::now();
		const auto response = windows_[currentLibraryIndex_]->getResponse();
		reactFunctions[response]();
		const auto t1 = std::chrono::high_resolution_clock::now();
		const auto timePassed = (t1-t0).count();
		const auto delta = normalLoopDuration - timePassed;
		if (delta > 0)
			usleep(delta);
	}
	usleep(normalLoopDuration * 3);
	windows_[currentLibraryIndex_]->closeWindow();
}

auto LogicUnit::initLibraries()
	-> std::array<ptrToLibraryType, nbLibraries>
{
	std::array<ptrToLibraryType, nbLibraries> libraries;
	const char* libraryNames[nbLibraries] =
		{ "libNcursesLib.dylib", "libDummyLib.dylib", "libSdlLib.dylib", "libSfmlLib.dylib"};
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
	snake_.fillMap(gameField_);
	windows_[currentLibraryIndex_]->draw(gameField_);
	endOfGame_ = snake_.collapsed();
}

void LogicUnit::reactToLeft()
{
	snake_.move(left);
	snake_.fillMap(gameField_);
	windows_[currentLibraryIndex_]->draw(gameField_);
	endOfGame_ = snake_.collapsed();
}

void LogicUnit::reactToRight()
{
	snake_.move(right);
	snake_.fillMap(gameField_);
	windows_[currentLibraryIndex_]->draw(gameField_);
	endOfGame_ = snake_.collapsed();
}

void LogicUnit::reactToUp()
{
	snake_.move(up);
	snake_.fillMap(gameField_);
	windows_[currentLibraryIndex_]->draw(gameField_);
	endOfGame_ = snake_.collapsed();
}

void LogicUnit::reactToDown()
{
	snake_.move(down);
	snake_.fillMap(gameField_);
	windows_[currentLibraryIndex_]->draw(gameField_);
	endOfGame_ = snake_.collapsed();
}

void LogicUnit::reactToToNcurses()
{
	windows_[currentLibraryIndex_]->closeWindow();
	currentLibraryIndex_ = ncurses;
	windows_[currentLibraryIndex_]->openWindow(width, height);
	windows_[currentLibraryIndex_]->draw(gameField_);
}

void LogicUnit::reactToToSDL()
{
	windows_[currentLibraryIndex_]->closeWindow();
	currentLibraryIndex_ = sdl;
	windows_[currentLibraryIndex_]->openWindow(width, height);
	windows_[currentLibraryIndex_]->draw(gameField_);
}

void LogicUnit::reactToToSFML()
{
	windows_[currentLibraryIndex_]->closeWindow();
	currentLibraryIndex_ = sfml;
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

void LogicUnit::pauseTheGame()
{
	//snake_.move(down);
	//snake_.fillMap(gameField_);
	paused_ = true;
	usleep(1000000);
	//windows_[currentLibraryIndex_]->draw(gameField_);
	endOfGame_ = false;
}

LogicUnit::~LogicUnit()
{
	for (auto& library : libraries_)
		dlclose(library);
}
