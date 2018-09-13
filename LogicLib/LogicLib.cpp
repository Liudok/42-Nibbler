#include "LogicLib.hpp"
#include <dlfcn.h>
#include <iostream>
#include <chrono>
#include <unistd.h>

LogicUnit::LogicUnit()
	: libraries_(initLibraries())
	, windows_(initWindows())
	, gameField_(height_, std::vector<size_t>(width_, 0))
	, snake_(gameField_[0].size(), gameField_.size())
{
	snake_.fillMap(gameField_);
}

void LogicUnit::loopTheGame()
{
	windows_[currentLibraryIndex_]->openWindow(width_, height_);
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
		std::bind(&LogicUnit::reactToPauseContinue, this) };
	const auto normalLoopDuration = 700000;
	while (!endOfGame_){
		const auto t0 = std::chrono::high_resolution_clock::now();
		const auto response = windows_[currentLibraryIndex_]->getResponse();
		if (paused_ && response != pauseContinue) continue;
		reactFunctions[response]();
		const auto t1 = std::chrono::high_resolution_clock::now();
		const auto timePassed = (t1-t0).count();
		const auto delta = normalLoopDuration - timePassed;
		windows_[currentLibraryIndex_]->setScore(snake_.getScore());
		windows_[currentLibraryIndex_]->setSpeed(speed_ + 10 * snake_.getScore());
		if (delta > 0)
			usleep(delta - 1000 * snake_.getScore());
	}
	windows_[currentLibraryIndex_]->showGameOver();
	usleep(normalLoopDuration * 3);
	windows_[currentLibraryIndex_]->closeWindow();
}

auto LogicUnit::initLibraries()
	-> std::array<ptrToLibraryType, nbLibraries>
{
	std::array<ptrToLibraryType, nbLibraries> libraries;
	const char* libraryNames[nbLibraries] =
		{ "libNcursesLib.dylib", "libSdlLib.dylib", "libSfmlLib.dylib"};
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
	windows_[currentLibraryIndex_]->openWindow(width_, height_);
	windows_[currentLibraryIndex_]->draw(gameField_);
}

void LogicUnit::reactToToSDL()
{
	windows_[currentLibraryIndex_]->closeWindow();
	currentLibraryIndex_ = sdl;
	windows_[currentLibraryIndex_]->openWindow(width_, height_);
	windows_[currentLibraryIndex_]->draw(gameField_);
}

void LogicUnit::reactToToSFML()
{
	windows_[currentLibraryIndex_]->closeWindow();
	currentLibraryIndex_ = sfml;
	windows_[currentLibraryIndex_]->openWindow(width_, height_);
	windows_[currentLibraryIndex_]->draw(gameField_);
}

void LogicUnit::reactToToDummy()
{
	windows_[currentLibraryIndex_]->closeWindow();
	currentLibraryIndex_ = sfml;
	windows_[currentLibraryIndex_]->openWindow(width_, height_);
	windows_[currentLibraryIndex_]->draw(gameField_);
}

void LogicUnit::reactToEndGame()
{
	windows_[currentLibraryIndex_]->closeWindow();
	endOfGame_ = true;
}

void LogicUnit::reactToPauseContinue()
{
	paused_ = !paused_;
}

size_t  LogicUnit::getWidth() const
{
	return width_;
}

size_t  LogicUnit::getHeight() const
{
	return height_;
}

void LogicUnit::setWindowSize(size_t w, size_t h)
{
	height_ = h;
	width_ = w;
	gameField_.clear();
	gameField_ = std::vector<std::vector<size_t>>(height_, std::vector<size_t>(width_, 0));
	snake_ = Snake(gameField_[0].size(), gameField_.size());
	snake_.fillMap(gameField_);
}

LogicUnit::LogicUnit(LogicUnit const& other)
 : libraries_(initLibraries())
 , windows_(initWindows())
 , gameField_(other.getHeight(), std::vector<size_t>(other.getWidth(), 0))
 , snake_(gameField_[0].size(), gameField_.size())
{
  setWindowSize(other.getWidth(), other.getHeight());
}

LogicUnit & LogicUnit::operator=(LogicUnit const& src)
{
  if (this != &src)
  {
	snake_ = src.snake_;
    gameField_ = src.gameField_;
  }
  return *this;
}

LogicUnit::~LogicUnit()
{
	for (auto& library : libraries_)
		dlclose(library);
}
