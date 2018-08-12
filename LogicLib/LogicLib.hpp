#pragma once
#include "../IWindow/IWindow.hpp"
#include "Snake.hpp"
#include <array>

int sampleSum(int a, int b);

class LogicUnit
{
public:
	LogicUnit();
	void loopTheGame();
	~LogicUnit();
private:
	enum libraryType { ncurses, dummy };
	static constexpr size_t nbLibraries = 2;
	static constexpr size_t height = 30;
	static constexpr size_t width = 50;
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
	void reactToToDummy();
	void reactToEndGame();
};
