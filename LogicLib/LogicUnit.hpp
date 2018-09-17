#pragma once

#include <IWindow.hpp>
#include "Snake.hpp"
#include <array>

class LogicUnit
{
  public:

    LogicUnit(size_t width, size_t height);
    LogicUnit(LogicUnit const&);
    LogicUnit &operator=(LogicUnit const &src);
    ~LogicUnit();

    bool    loopTheGame();

  private:

    using ptrToLibraryType = void*;
    using windowPtr = std::unique_ptr<IWindow>;
    using libraryIndex = size_t;
    enum libraryType { ncurses, sdl, sfml };
    static constexpr size_t nbLibraries = 3;

    std::array<ptrToLibraryType, nbLibraries> initLibraries();
    std::vector<windowPtr> initWindows();
    std::array<ptrToLibraryType, nbLibraries> libraries_;
    std::vector<windowPtr> windows_;
    libraryIndex currentLibraryIndex_ = sfml;

    size_t     width_ = 30;
    size_t     height_ = 50;
    bool       endOfGame_ = false;
    bool       paused_ = false;
    gameField  gameField_;
    Snake      snake_;

    void       reactToNoResponse();
    void       reactToNewDirection(responseType);
    void       reactToNewLibrary(libraryType);
    void       reactToToSDL();
    void       reactToEndGame();
    void       reactToPauseContinue();

    size_t     countUsleep(int timePassed);

};