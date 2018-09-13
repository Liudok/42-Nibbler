#pragma once

#include <vector>

enum responseType { noResponse, left, right, up, down,
		toNcurses, toDummy, toSDL, toSFML, endGame, pauseContinue };

constexpr size_t nbResponses = 11;

enum gameFieldCellType { empty, body, head, food, collision };

class IWindow
{
public:
	virtual responseType getResponse() = 0;
	virtual void draw(std::vector<std::vector<size_t>> const&) = 0;
	virtual void openWindow(size_t width, size_t height) = 0;
	virtual void closeWindow() = 0;
	virtual void setScore(size_t score) = 0;
	virtual void setSpeed(size_t speed) = 0;
	virtual void showGameOver() = 0;
	virtual ~IWindow() {}
};
