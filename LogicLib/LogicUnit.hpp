#pragma once

#include <Nibbler.hpp>
#include "Handle.hpp"
#include "Snake.hpp"
#include <array>

class LogicUnit
{
  public:

    LogicUnit(NibblerParameters);
    bool loopTheGame();

    LogicUnit(LogicUnit const&) = delete;
    LogicUnit& operator=(LogicUnit const&) = delete;
    ~LogicUnit();

  private:

    using WindowsArray = std::array<Handle<IWindow>, nbLibraries>;

    NibblerParameters params_{defaultWidth, defaultHeight, classic};

    WindowsArray windows_ = {{
        "libGlfwLib.dylib",
        "libSdlLib.dylib",
        "libSfmlLib.dylib"
    }};
    size_t currentLibraryIndex_ = params_.lib;

    using MusicPlayer = Handle<IMusicPlayer>;
    MusicPlayer musicPlayer_ = "libSoundLib.dylib";

    bool playerPressedEscape_ = false;
    bool paused_ = false;
    GameField gameField_ = GameField(params_.height,
        std::vector<size_t>(params_.width, 0));
    Snake snake_ = Snake(params_, musicPlayer_);

    void reactToNoResponse();
    void reactToNewDirection(ResponseType);
    void reactToNewLibrary(LibraryType);
    void reactToPlayerPressedEscape();
    void reactToPauseContinue();
    void reactToChangeGameMode();

    void showExitAndCloseWindow();

    size_t countUsleep(int timePassed) const;

    using ReactFunctionsArray =
        std::array<std::function<void()>,nbResponses>;
    ReactFunctionsArray initReactFunctionsArray();

    inline bool allowedActionWhilePaused(ResponseType t) const
    { return !(t >= noResponse && t <= down); }

};
