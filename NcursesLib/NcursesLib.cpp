#include "NcursesLib.hpp"

extern "C"
{
    IWindow* create()
    {
        return new NcuresesWindow;
    }
}

ResponseType NcuresesWindow::getResponse()
{
    const auto response = wgetch(window_);
    switch(response){
        case 'q': return playerPressedEscape;
        case 'a': return left;
        case KEY_LEFT: return left;
        case 'd': return right;
        case KEY_RIGHT: return right;
        case 'w': return up;
        case KEY_UP: return up;
        case 's': return down;
        case KEY_DOWN: return down;
        case '1' : return toSFML;
        case '2' : return toSDL;
        case ' ': return pauseContinue;
        default: return noResponse;
    }
}

void NcuresesWindow::draw(GameField const& gameState, size_t, size_t, GameMode)
{
    drawGameState(window_, gameState);
    wrefresh(window_);
}

void NcuresesWindow::openWindow(size_t width, size_t height)
{
    width_ = width;
    height_ = height;
    initscr();
    window_ = newwin(height_ + 2, width_ + 2, 0, 0);
    keypad(window_, TRUE);
    nodelay(window_, TRUE);
    box(window_, 0, 0);
    curs_set(0);
    start_color();
    init_pair(empty, COLOR_WHITE, COLOR_BLACK);
    init_pair(body, COLOR_GREEN, COLOR_BLACK);
    init_pair(head, COLOR_GREEN, COLOR_GREEN);
    init_pair(food, COLOR_MAGENTA, COLOR_BLACK);
    init_pair(collision, COLOR_RED, COLOR_RED);
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
    GameField const& gameState)
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
