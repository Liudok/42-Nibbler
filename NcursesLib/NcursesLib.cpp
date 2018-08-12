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
	window_ = newwin(height_ + 2, width_ + 2, 0, 0);
	drowGameState(window_, gameState);
	keypad(window_, TRUE);
	nodelay(window_, FALSE);
	box(window_, 0, 0);
	initscr();
	curs_set(0);
	wrefresh(window_);
}

void NcuresesWindow::openWindow(size_t width, size_t height)
{
	width_ = width;
	height_ = height;
}

void NcuresesWindow::closeWindow()
{
	delwin(window_);
	endwin();
}

void NcuresesWindow::drowGameState(WINDOW* window_,
	std::vector<std::vector<size_t>> const& gameState)
{
	for (size_t i = 0; i < height_; ++i){
		for (size_t j = 0; j < width_; ++j){
			if (gameState[i][j]) wattron(window_, A_BOLD);
			mvwprintw(window_, i + 1, j + 1, "%i", gameState[i][j]);
			wattroff(window_, A_BOLD);
		}
	}
}
