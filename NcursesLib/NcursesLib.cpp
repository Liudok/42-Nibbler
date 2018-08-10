#include "NcursesLib.hpp"
#include <unistd.h>
#include <iostream>

extern "C"
{
	IWindow* create()
	{
		return new NcuresesWindow;
	}
}

responseType NcuresesWindow::getResponse()
{
	std::cout << "getResponse of the ncurses dll called" << std::endl;
	return noResponse;
}

void NcuresesWindow::drow(std::vector<std::vector<size_t>> const& gameState)
{
	std::cout << "Drow of the ncurses dll called" << std::endl;
	window_ = newwin(height_, width_, 0, 0);
	keypad(window_, TRUE);
	drowGameState(window_, gameState);
	wrefresh(window_);
	sleep(1);
}

void NcuresesWindow::openWindow()
{
	initscr();
	curs_set(0);
}

void NcuresesWindow::openWindow(size_t width, size_t height)
{
	width_ = width;
	height_ = height;
	initscr();
	curs_set(0);
}

void NcuresesWindow::closeWindow()
{
	delwin(window_);
	endwin();
}

void NcuresesWindow::drowGameState(WINDOW* window_,
	std::vector<std::vector<size_t>> const& gameState)
{
	const size_t nbRows = gameState.size();
	const size_t nbColumns = gameState[0].size();
	for (size_t i = 0; i < nbRows; ++i)
		for (size_t j = 0; j < nbColumns; ++j)
			mvwprintw(window_, i, j, "%i", gameState[i][j]);
}
