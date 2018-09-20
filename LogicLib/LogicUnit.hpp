#pragma once

#include <Nibbler.hpp>
#include "Snake.hpp"
#include <array>

class LogicUnit
{
  public:

    LogicUnit(NibblerParameters);
    LogicUnit(LogicUnit const&) = delete;
    LogicUnit& operator=(LogicUnit const&) = delete;
    ~LogicUnit();

    bool loopTheGame();

  private:

    using PtrToLibraryType = void*;
    using WindowUPtr = std::unique_ptr<IWindow>;
    using LibraryIndex = size_t;
    using LibrariesArray = std::array<PtrToLibraryType, nbLibraries>;
    using WindowsArray = std::vector<WindowUPtr>;
    using MusicPlayerSPtr = std::shared_ptr<IMusicPlayer>;

    LibrariesArray initLibraries();
    WindowsArray initWindows();

    NibblerParameters params_{defaultWidth, defaultHeight, classic};
    LibrariesArray libraries_ = initLibraries();
    WindowsArray windows_ = initWindows();
    LibraryIndex currentLibraryIndex_ = params_.lib;

    static constexpr auto musicLibraryName_ = "libSoundLib.dylib";
    PtrToLibraryType musicLibrary_ = openLibrary(musicLibraryName_);
    MusicPlayerSPtr musicPlayer_ = makeMusicPlayer();

    bool playerPressedEscape_ = false;
    bool paused_ = false;
    GameField gameField_ = GameField(params_.height,
        std::vector<size_t>(params_.width, 0));
    Snake snake_ = Snake(params_, musicPlayer_);

    void reactToNoResponse();
    void reactToNewDirection(ResponseType);
    void reactToNewLibrary(LibraryType);
    void reactToplayerPressedEscape();
    void reactToPauseContinue();
    void reactToChangeGameMode();

    void showExitAndCloseWindow();

    size_t countUsleep(int timePassed) const;
    static PtrToLibraryType openLibrary(const char* libraryName);
    MusicPlayerSPtr makeMusicPlayer() const;

    using ReactFunctionsArray =
        std::array<std::function<void()>,nbResponses>;
    ReactFunctionsArray initReactFunctionsArray();

    inline bool allowedActionWhilePaused(ResponseType t) const
    { return !(t >= noResponse && t <= down); }

};
