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
private:
	void drowGameState(WINDOW* window,
		std::vector<std::vector<size_t>> const& gameState);
};
