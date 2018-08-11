#include "DummyLib.hpp"
#include <iostream>

extern "C"
{
	IWindow* create()
	{
		return new DummyWindow;
	}
}

responseType DummyWindow::getResponse()
{
	std::cout << "Hit one of those:" << std::endl;
	std::cout << "\'q\' - to quit" << std::endl;
	std::cout << "\'a\' - to move left" << std::endl;
	std::cout << "\'d\' - to move right" << std::endl;
	std::cout << "\'w\' - to move up" << std::endl;
	std::cout << "\'s\' - to move down" << std::endl;
	std::cout << "\'z\' - to switch to ncurses mode" << std::endl;
	std::cout << "\'q\' - to switch to dummy mode" << std::endl;
	std::cout << "And then hit Enter..." << std::endl;
	const auto response = std::cin.get();
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

void DummyWindow::drow(std::vector<std::vector<size_t>> const& gameState)
{
	std::cout << "Drow of the dummy dll called" << std::endl;
	for (const auto& line : gameState){
		for (const auto cell : line)
			std::cout << cell;
		std::cout << std::endl;
	}
}

void DummyWindow::openWindow(size_t, size_t)
{
	std::cout << "openWindow of the dummy dll called" << std::endl;
}

void DummyWindow::closeWindow()
{
	std::cout << "closeWindow of the dummy dll called" << std::endl;
}
