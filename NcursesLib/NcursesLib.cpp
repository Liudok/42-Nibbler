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
	initscr();
	const size_t nbRows = gameState.size();
	const size_t nbColumns = gameState[0].size();
	auto window = newwin(nbRows, nbColumns, 0, 0);
	curs_set(0);
	keypad(window, TRUE);
	drowGameState(window, gameState);
	wrefresh(window);
	sleep(1);
	delwin(window);
	endwin();
}

void NcuresesWindow::drowGameState(WINDOW* window,
	std::vector<std::vector<size_t>> const& gameState)
{
	const size_t nbRows = gameState.size();
	const size_t nbColumns = gameState[0].size();
	for (size_t i = 0; i < nbRows; ++i)
		for (size_t j = 0; j < nbColumns; ++j)
			mvwprintw(window, i, j, "%i", gameState[i][j]);
}
