#include "NcursesLib.hpp"

extern "C"
{
	IWindow* create()
	{
		return new NcuresesWindow;
	}
}

responseType NcuresesWindow::getResponse()
{
	const auto response = wgetch(window_);
	switch(response){
		case ' ': return noResponse;
		case 'q': return endGame;
		case 'a': return left;
		case 'd': return right;
		case 'w': return up;
		case 's': return down;
		case 'z': return toNcurses;
		case 'x': return toDummy;
		default: return noResponse;
	}
}

void NcuresesWindow::drow(std::vector<std::vector<size_t>> const& gameState)
{
	window_ = newwin(height_, width_, 0, 0);
	keypad(window_, TRUE);
	nodelay(window_, FALSE);
	drowGameState(window_, gameState);
	wrefresh(window_);
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
