#pragma once

#include <IWindow.hpp>
#include "Snake.hpp"
#include <array>

<<<<<<< HEAD
using ptrToLibraryType = void*;
using windowPtr = std::unique_ptr<IWindow>;
using libraryIndex = size_t;
enum libraryType { ncurses, sdl, sfml };
static constexpr size_t nbLibraries = 3;

class LogicUnit
{
  public:

    LogicUnit();
    LogicUnit(LogicUnit const & other);
    ~LogicUnit();
    LogicUnit &operator=(LogicUnit const &src);
    void    loopTheGame();
    void    setWindowSize(size_t width, size_t height);
    size_t  getWidth() const;
    size_t  getHeight() const;

  private:

    std::array<ptrToLibraryType, nbLibraries> initLibraries();
    std::vector<windowPtr> initWindows();
    std::array<ptrToLibraryType, nbLibraries> libraries_;
    std::vector<windowPtr> windows_;
    libraryIndex currentLibraryIndex_ = sfml;
    size_t     height_ = 50;
    size_t     width_ = 30;
    size_t     speed_ = 10;
    gameField  gameField_;
    Snake      snake_;
    bool       endOfGame_ = false;
    bool       paused_ = false;
    void       reactToNoResponse();
    void       reactToLeft();
    void       reactToRight();
    void       reactToUp();
    void       reactToDown();
    void       reactToToNcurses();
    void       reactToToSDL();
    void       reactToToSFML();
    void       reactToToDummy();
    void       reactToEndGame();
    void       pauseTheGame();

=======
class LogicUnit
{
public:
	LogicUnit();
	void loopTheGame();
	~LogicUnit();
private:
	enum libraryType { ncurses, dummy, sdl, sfml};
	static constexpr size_t nbLibraries = 4;
	static constexpr size_t height = 50;
	static constexpr size_t width = 30;
	using ptrToLibraryType = void*;
	using windowPtr = std::unique_ptr<IWindow>;
	using libraryIndex = size_t;
	using gameField = std::vector<std::vector<size_t>>;
	std::array<ptrToLibraryType, nbLibraries> initLibraries();
	std::vector<windowPtr> initWindows();
	std::array<ptrToLibraryType, nbLibraries> libraries_;
	std::vector<windowPtr> windows_;
	libraryIndex currentLibraryIndex_ = ncurses;
	gameField gameField_;
	Snake snake_;
	bool endOfGame_ = false;
	void reactToNoResponse();
	void reactToLeft();
	void reactToRight();
	void reactToUp();
	void reactToDown();
	void reactToToNcurses();
	void reactToToSDL();
	void reactToToSFML();
	void reactToToDummy();
	void reactToEndGame();
>>>>>>> master
};
