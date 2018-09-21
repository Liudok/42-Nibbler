#include "LogicUnit.hpp"
#include <chrono>
#include <unistd.h>
#include <string>

LogicUnit::LogicUnit(NibblerParameters params)
    : params_(params)
{
    snake_.fillMap(gameField_);
    if (musicPlayer_.get() && params_.player == on)
        musicPlayer_.get()->playMainTheme();
}

LogicUnit::~LogicUnit()
{
    if (musicPlayer_.get() && params_.player == on)
        musicPlayer_.get()->stopMainTheme();
}

bool LogicUnit::loopTheGame()
{
    windows_[currentLibraryIndex_].get()->openWindow(params_.width, params_.height);
    windows_[currentLibraryIndex_].get()->draw(gameField_,
        snake_.getScore(), snake_.getSpeed(), params_.mode);
    const auto reactFunctions = initReactFunctionsArray();
    while (true){
        const auto t0 = std::chrono::steady_clock::now();
        const auto response = windows_[currentLibraryIndex_].get()->getResponse();
        if (paused_ && !(allowedActionWhilePaused(response))) continue;
        reactFunctions[response]();
        const auto t1 = std::chrono::steady_clock::now();
        const auto timePassed =
            std::chrono::duration_cast<std::chrono::microseconds>(t1-t0).count();
        usleep(countUsleep(timePassed));
        if (playerPressedEscape_) break;
        if (snake_.collapsed()){
            showExitAndCloseWindow();
            break;
        }
    }
    return !playerPressedEscape_;
}

auto LogicUnit::initWindows() const
    -> WindowsArray
{
    using namespace NibblerUtils;
    return{{
        mergePath(getPathToBuildDir(), "/libGlfwLib.dylib").c_str(),
        mergePath(getPathToBuildDir(), "/libSdlLib.dylib").c_str(),
        mergePath(getPathToBuildDir(), "/libSfmlLib.dylib").c_str()
    }};
}

void LogicUnit::reactToNoResponse()
{
    snake_.move();
    snake_.fillMap(gameField_);
    windows_[currentLibraryIndex_].get()->draw(gameField_,
        snake_.getScore(), snake_.getSpeed(), params_.mode);
}

void LogicUnit::reactToNewDirection(ResponseType newDirection)
{
    snake_.move(newDirection);
    snake_.fillMap(gameField_);
    windows_[currentLibraryIndex_].get()->draw(gameField_,
        snake_.getScore(), snake_.getSpeed(), params_.mode);
}

void LogicUnit::reactToNewLibrary(LibraryType newLibrary)
{
    windows_[currentLibraryIndex_].get()->closeWindow();
    currentLibraryIndex_ = newLibrary;
    windows_[currentLibraryIndex_].get()->openWindow(params_.width, params_.height);
    windows_[currentLibraryIndex_].get()->draw(gameField_,
        snake_.getScore(), snake_.getSpeed(), params_.mode);
}

void LogicUnit::reactToPlayerPressedEscape()
{
    showExitAndCloseWindow();
    playerPressedEscape_ = true;
}

void LogicUnit::reactToPauseContinue()
{
    paused_ = !paused_;
    if (musicPlayer_.get() && params_.player == on){
        if (paused_)
            musicPlayer_.get()->pauseMainTheme();
        else
            musicPlayer_.get()->playMainTheme();
    }
}

void LogicUnit::reactToChangeGameMode()
{
    params_.mode = static_cast<GameMode>(
        (params_.mode + 1) % nbGameModes);
}

void LogicUnit::showExitAndCloseWindow()
{
    windows_[currentLibraryIndex_].get()->showGameOver();
    if (params_.player != off)
        musicPlayer_.get()->playSound(gameOver);
    usleep(2'500'000);
    windows_[currentLibraryIndex_].get()->closeWindow();
}

size_t LogicUnit::countUsleep(int timePassed) const
{
    const auto loopNormalDuration = 100'000 *
        ((params_.mode == granny) ? 3 : 1) /
        ((params_.mode == insane) ? 3 : 1);
    const auto delta = loopNormalDuration - timePassed;
    return (delta < 0) ? 0 : delta / snake_.getSpeed();
}

auto LogicUnit::initReactFunctionsArray()
    -> ReactFunctionsArray
{
    return {{
        [this]{ reactToNoResponse(); },
        [this]{ reactToNewDirection(left); },
        [this]{ reactToNewDirection(right); },
        [this]{ reactToNewDirection(up); },
        [this]{ reactToNewDirection(down); },
        [this]{ reactToNewLibrary(glfw); },
        [this]{ reactToNewLibrary(sdl); },
        [this]{ reactToNewLibrary(sfml); },
        [this]{ reactToPauseContinue(); },
        [this]{ reactToChangeGameMode(); },
        [this]{ reactToPlayerPressedEscape(); },
    }};
}

std::string LogicUnit::mergePath(const char* left, const char* right) const
{
    std::string result = left;
    result += right;
    return result;
}
