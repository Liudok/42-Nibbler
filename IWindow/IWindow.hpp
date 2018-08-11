#pragma once
#include <vector>

enum responseType { noResponse, left, right, up, down,
		toNcurses, toDummy, endGame };

enum gameFieldCellType { empty, body, head, food };

class IWindow
{
public:
	virtual responseType getResponse() = 0;
	virtual void drow(std::vector<std::vector<size_t>> const&) = 0;
	virtual void openWindow(size_t width, size_t height) = 0;
	virtual void closeWindow() = 0;
	virtual ~IWindow() {}
};
