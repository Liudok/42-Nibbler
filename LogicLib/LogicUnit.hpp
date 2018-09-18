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
    enum LibraryType { ncurses, sdl, sfml };
    static constexpr size_t nbLibraries = 3;

    std::array<PtrToLibraryType, nbLibraries> initLibraries();
    std::vector<WindowPtr> initWindows();

    std::array<PtrToLibraryType, nbLibraries> libraries_ = initLibraries();
    std::vector<WindowPtr> windows_ = initWindows();
    LibraryIndex currentLibraryIndex_ = sfml;

    static constexpr auto musicLibraryName_ = "libSoundLib.dylib";
    PtrToLibraryType musicLibrary_ = openLibrary(musicLibraryName_);
    MusicPlayerPtr musicPlayer_ = makeMusicPlayer();

    NibblerParameters params_{40, 50, classic};

    bool playerPressedEscape_ = false;
    bool paused_ = false;
    GameField gameField_;
    Snake snake_;

    void       reactToNoResponse();
    void       reactToNewDirection(ResponseType);
    void       reactToNewLibrary(LibraryType);
    void       reactToPlayerPressedEscape();
    void       reactToPauseContinue();

    size_t     countUsleep(int timePassed) const;

    static PtrToLibraryType openLibrary(const char* libraryName);
    MusicPlayerPtr makeMusicPlayer();
    void showExitAndCloseWindow();

};
