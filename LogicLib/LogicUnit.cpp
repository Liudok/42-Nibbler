#include "LogicUnit.hpp"
#include <dlfcn.h>
#include <chrono>
#include <unistd.h>

LogicUnit::LogicUnit(NibblerParameters params)
    : params_(params)
{
    snake_.fillMap(gameField_);
    if (params_.player == on)
        musicPlayer_->playMainTheme();
}

LogicUnit::~LogicUnit()
{
    for (auto& library : libraries_)
        dlclose(library);
    if (params_.player == on)
        musicPlayer_->stopMainTheme();
    dlclose(musicLibrary_);
}

bool LogicUnit::loopTheGame()
{
    windows_[currentLibraryIndex_]->openWindow(params_.width, params_.height);
    windows_[currentLibraryIndex_]->draw(gameField_,
        snake_.getScore(), snake_.getSpeed(), params_.mode);
    const auto reactFunctions = initReactFunctionsArray();
    while (true){
        const auto t0 = std::chrono::steady_clock::now();
        const auto response = windows_[currentLibraryIndex_]->getResponse();
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

auto LogicUnit::initLibraries()
    -> LibrariesArray
{
    LibrariesArray libraries;
    const char* libraryNames[nbLibraries] =
        { "libGlfwLib.dylib", "libSdlLib.dylib", "libSfmlLib.dylib"};
    for (size_t currentLib = 0; currentLib < nbLibraries; ++currentLib)
        libraries[currentLib] = openLibrary(libraryNames[currentLib]);
    return libraries;
}

auto LogicUnit::initWindows()
    -> WindowsArray
{
    WindowsArray result;
    const auto factoryFunctionName = "create";
    using factoryFunctionType = IWindow* (*)();
    for (size_t i = 0; i < nbLibraries; ++i){
        auto factoryFunctionPtr =
            reinterpret_cast<factoryFunctionType>(dlsym(libraries_[i],
                factoryFunctionName));
        result.push_back(WindowUPtr{factoryFunctionPtr()});
    }
    return result;
}

void LogicUnit::reactToNoResponse()
{
    snake_.move();
    snake_.fillMap(gameField_);
    windows_[currentLibraryIndex_]->draw(gameField_,
        snake_.getScore(), snake_.getSpeed(), params_.mode);
}

void LogicUnit::reactToNewDirection(ResponseType newDirection)
{
    snake_.move(newDirection);
    snake_.fillMap(gameField_);
    windows_[currentLibraryIndex_]->draw(gameField_,
        snake_.getScore(), snake_.getSpeed(), params_.mode);
}

void LogicUnit::reactToNewLibrary(LibraryType newLibrary)
{
    windows_[currentLibraryIndex_]->closeWindow();
    currentLibraryIndex_ = newLibrary;
    windows_[currentLibraryIndex_]->openWindow(params_.width, params_.height);
    windows_[currentLibraryIndex_]->draw(gameField_,
        snake_.getScore(), snake_.getSpeed(), params_.mode);
}

void LogicUnit::reactToplayerPressedEscape()
{
    showExitAndCloseWindow();
    playerPressedEscape_ = true;
}

void LogicUnit::reactToPauseContinue()
{
    paused_ = !paused_;
    if (params_.player == on){
        if (paused_)
            musicPlayer_->pauseMainTheme();
        else
            musicPlayer_->playMainTheme();
    }
}

void LogicUnit::reactToChangeGameMode()
{
    params_.mode = static_cast<GameMode>(
        (params_.mode + 1) % nbGameModes);
}

void LogicUnit::showExitAndCloseWindow()
{
    windows_[currentLibraryIndex_]->showGameOver();
    if (params_.player != off)
        musicPlayer_->playSound(gameOver);
    usleep(2'500'000);
    windows_[currentLibraryIndex_]->closeWindow();
}

size_t LogicUnit::countUsleep(int timePassed) const
{
    const auto loopNormalDuration = 100'000 *
        ((params_.mode == granny) ? 3 : 1) /
        ((params_.mode == insane) ? 3 : 1);
    const auto delta = loopNormalDuration - timePassed;
    return (delta < 0) ? 0 : delta / snake_.getSpeed();
}

auto LogicUnit::openLibrary(const char* libraryName)
    -> PtrToLibraryType
{
    const auto result =
            dlopen(libraryName, RTLD_LAZY | RTLD_LOCAL);
    if (!result)
        throw std::runtime_error("The dl is not found");
    return result;
}

auto LogicUnit::makeMusicPlayer() const
    -> MusicPlayerSPtr
{
    const auto factoryFunctionName = "create";
    using factoryFunctionType = IMusicPlayer* (*)();
    auto factoryFunctionPtr =
            reinterpret_cast<factoryFunctionType>(dlsym(musicLibrary_,
                factoryFunctionName));
    return MusicPlayerSPtr{factoryFunctionPtr()};
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
        [this]{ reactToplayerPressedEscape(); },
    }};
}
