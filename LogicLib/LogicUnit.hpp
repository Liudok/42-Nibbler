#pragma once

#include <IWindow.hpp>
#include "Snake.hpp"
#include <IMusicPlayer.hpp>
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
    using WindowPtr = std::unique_ptr<IWindow>;
    using MusicPlayerPtr = std::shared_ptr<IMusicPlayer>;
    using LibraryIndex = size_t;

    std::array<PtrToLibraryType, nbLibraries> initLibraries();
    std::vector<WindowPtr> initWindows();

    NibblerParameters params_{40, 50, classic};

    std::array<PtrToLibraryType, nbLibraries> libraries_ = initLibraries();
    std::vector<WindowPtr> windows_ = initWindows();
    LibraryIndex currentLibraryIndex_ = params_.lib;

    static constexpr auto musicLibraryName_ = "libSoundLib.dylib";
    PtrToLibraryType musicLibrary_ = openLibrary(musicLibraryName_);
    MusicPlayerPtr musicPlayer_ = makeMusicPlayer();

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

    size_t countUsleep(int timePassed) const;

    static PtrToLibraryType openLibrary(const char* libraryName);
    MusicPlayerPtr makeMusicPlayer();
    void showExitAndCloseWindow();

};
