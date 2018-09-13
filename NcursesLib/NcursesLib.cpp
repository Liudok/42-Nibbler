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
		case KEY_LEFT: return left;
		case 'd': return right;
		case KEY_RIGHT: return right;
		case 'w': return up;
		case KEY_UP: return up;
		case 's': return down;
<<<<<<< HEAD
		case KEY_DOWN: return down;
		case '1' : return toSFML;
		case '2' : return toSDL;
=======
		case 'z': return toNcurses;
		case 'x': return toDummy;
		case ' ': return pauseContinue;
>>>>>>> master
		default: return noResponse;
	}
}

void NcuresesWindow::draw(std::vector<std::vector<size_t>> const& gameState)
{
	drawGameState(window_, gameState);
	wrefresh(window_);
}

void NcuresesWindow::openWindow(size_t width, size_t height)
{
	width_ = width;
	height_ = height;
}

void NcuresesWindow::setScore(size_t score)
{
	score_ = score;
}

void NcuresesWindow::setSpeed(size_t speed)
{
	speed_ = speed;
}

void NcuresesWindow::showGameOver()
{
	
}

void NcuresesWindow::closeWindow()
{
	delwin(window_);
	window_ = nullptr;
	endwin();
}

void NcuresesWindow::drawGameState(WINDOW* window_,
	std::vector<std::vector<size_t>> const& gameState)
{
	for (size_t i = 0; i < height_; ++i){
		for (size_t j = 0; j < width_; ++j){
			if (gameState[i][j]) wattron(window_, A_BOLD);
			wattron(window_, COLOR_PAIR(gameState[i][j]));
			mvwprintw(window_, i + 1, j + 1, "%i", gameState[i][j]);
			wattroff(window_, A_BOLD);
			wattroff(window_, COLOR_PAIR(gameState[i][j]));
		}
	}
}
