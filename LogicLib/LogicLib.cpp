#include "LogicLib.hpp"
#include <dlfcn.h>
#include <iostream>
#include <chrono>
#include <unistd.h>

LogicUnit::LogicUnit(size_t height, size_t width)
    : libraries_(initLibraries())
    , windows_(initWindows())
    , width_(width)
    , height_(height)
    , gameField_(height_, std::vector<size_t>(width_, 0))
    , snake_(width_, height_)
{
    snake_.fillMap(gameField_);
}

LogicUnit::LogicUnit(LogicUnit const& other)
 : libraries_(initLibraries())
 , windows_(initWindows())
 , gameField_(other.getHeight(), std::vector<size_t>(other.getWidth(), 0))
 , snake_(gameField_[0].size(), gameField_.size())
{

}

LogicUnit& LogicUnit::operator=(LogicUnit const& src)
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

void LogicUnit::loopTheGame()
{
    windows_[currentLibraryIndex_]->openWindow(width_, height_);
    windows_[currentLibraryIndex_]->draw(gameField_,
        snake_.getScore(), snake_.getSpeed());
    std::function<void()> reactFunctions[nbResponses] = {
        [this]{ reactToNoResponse(); },
        [this]{ reactToLeft(); },
        [this]{ reactToRight(); },
        [this]{ reactToUp(); },
        [this]{ reactToDown(); },
        [this]{ reactToToNcurses(); },
        [this]{ reactToToSDL(); },
        [this]{ reactToToSFML(); },
        [this]{ reactToPauseContinue(); },
        [this]{ reactToEndGame(); },
    };
    while (!endOfGame_){
        const auto t0 = std::chrono::steady_clock::now();
        const auto response = windows_[currentLibraryIndex_]->getResponse();
        if (paused_ && response != pauseContinue) continue;
        reactFunctions[response]();
        const auto t1 = std::chrono::steady_clock::now();
        const auto timePassed =
            std::chrono::duration_cast<std::chrono::microseconds>(t1-t0).count();
        usleep(countUsleep(timePassed));
    }
    windows_[currentLibraryIndex_]->showGameOver();
    usleep(500'000);
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
    windows_[currentLibraryIndex_]->draw(gameField_,
        snake_.getScore(), snake_.getSpeed());
    endOfGame_ = snake_.collapsed();
}

void LogicUnit::reactToLeft()
{
    snake_.move(left);
    snake_.fillMap(gameField_);
    windows_[currentLibraryIndex_]->draw(gameField_,
        snake_.getScore(), snake_.getSpeed());
    endOfGame_ = snake_.collapsed();
}

void LogicUnit::reactToRight()
{
    snake_.move(right);
    snake_.fillMap(gameField_);
    windows_[currentLibraryIndex_]->draw(gameField_,
        snake_.getScore(), snake_.getSpeed());
    endOfGame_ = snake_.collapsed();
}

void LogicUnit::reactToUp()
{
    snake_.move(up);
    snake_.fillMap(gameField_);
    windows_[currentLibraryIndex_]->draw(gameField_,
        snake_.getScore(), snake_.getSpeed());
    endOfGame_ = snake_.collapsed();
}

void LogicUnit::reactToDown()
{
    snake_.move(down);
    snake_.fillMap(gameField_);
    windows_[currentLibraryIndex_]->draw(gameField_,
        snake_.getScore(), snake_.getSpeed());
    endOfGame_ = snake_.collapsed();
}

void LogicUnit::reactToToNcurses()
{
    windows_[currentLibraryIndex_]->closeWindow();
    currentLibraryIndex_ = ncurses;
    windows_[currentLibraryIndex_]->openWindow(width_, height_);
    windows_[currentLibraryIndex_]->draw(gameField_,
        snake_.getScore(), snake_.getSpeed());
}

void LogicUnit::reactToToSDL()
{
    windows_[currentLibraryIndex_]->closeWindow();
    currentLibraryIndex_ = sdl;
    windows_[currentLibraryIndex_]->openWindow(width_, height_);
    windows_[currentLibraryIndex_]->draw(gameField_,
        snake_.getScore(), snake_.getSpeed());
}

void LogicUnit::reactToToSFML()
{
    windows_[currentLibraryIndex_]->closeWindow();
    currentLibraryIndex_ = sfml;
    windows_[currentLibraryIndex_]->openWindow(width_, height_);
    windows_[currentLibraryIndex_]->draw(gameField_,
        snake_.getScore(), snake_.getSpeed());
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

size_t LogicUnit::countUsleep(int timePassed)
{
    const auto loopNormalDuration = 100'000;
    const auto delta = loopNormalDuration - timePassed;
    return (delta < 0) ? 0 : delta / snake_.getSpeed();
}
