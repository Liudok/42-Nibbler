#pragma once
#include "../IWindow/IWindow.hpp"
#include <array>

int sampleSum(int a, int b);

struct Point
{
	size_t x = 0;
	size_t y = 0;
};

class LogicUnit
{
public:
	LogicUnit();
	void loopTheGame();
	~LogicUnit();
private:
	enum libraryType { ncurses, dummy };
	enum gameStateType { empty, body, head, food };
	static constexpr size_t nbLibraries = 2;
	static constexpr size_t height = 6;
	static constexpr size_t width = 10;
	using ptrToLibraryType = void*;
	using windowPtr = std::unique_ptr<IWindow>;
	using libraryIndex = size_t;
	std::array<ptrToLibraryType, nbLibraries> initLibraries();
	std::vector<windowPtr> initWindows();
	void updateGameState(responseType);
	std::array<ptrToLibraryType, nbLibraries> libraries_;
	std::vector<windowPtr> windows_;
	libraryIndex currentLibraryIndex_ = 0;
	std::vector<std::vector<size_t>> gameState_;
	Point headPos_;
};
