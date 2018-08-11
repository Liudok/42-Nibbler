#pragma once
#include "../IWindow/IWindow.hpp"
#include <ncurses.h>

extern "C"
{
	IWindow* create();
}

class NcuresesWindow : public IWindow
{
public:
	responseType getResponse() override;
	void drow(std::vector<std::vector<size_t>> const&) override;
	void openWindow(size_t width, size_t height) override;
	void closeWindow() override;
private:
	void drowGameState(WINDOW* window,
		std::vector<std::vector<size_t>> const& gameState);
	WINDOW* window_ = nullptr;
	size_t width_ = 0;
	size_t height_ = 0;
};
