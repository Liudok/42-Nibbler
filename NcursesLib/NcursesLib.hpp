#pragma once

#include <IWindow.hpp>
#include <ncurses.h>

extern "C"
{
	IWindow* create();
}

class NcuresesWindow : public IWindow
{
public:
	responseType getResponse() override;
	void draw(field const&, size_t score, size_t speed) override;
	void openWindow(size_t width, size_t height) override;
	void closeWindow() override;
	void showGameOver() override;
private:
	void drawGameState(WINDOW* window, field const& gameState);
	WINDOW* window_ = nullptr;
	size_t width_ = 0;
	size_t height_ = 0;
};
