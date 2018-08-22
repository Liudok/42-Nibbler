#pragma once

#include <IWindow.hpp>
#include "Snake.hpp"
#include <array>

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
	libraryIndex currentLibraryIndex_ = sfml;
	gameField gameField_;
	Snake snake_;
	bool endOfGame_ = false;
	bool paused_ = false;
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
	void pauseTheGame();
};
