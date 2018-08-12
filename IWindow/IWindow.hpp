#pragma once
#include <vector>

enum responseType { noResponse, left, right, up, down,
		toNcurses, toDummy, endGame };

constexpr size_t nbResponses = 8;

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
