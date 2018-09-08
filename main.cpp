#include "LogicLib/LogicLib.hpp"
#include <iostream>

int main(int ac, char **av)
{
	if (ac > 3 || ac == 2)
	{
		std::cout << "./Build/Nibbler [width] [height]" << std::endl;
	}
	try{
		LogicUnit game;
		if (ac == 3)
			{
				size_t w = static_cast<size_t>(std::stoi(av[1]));
				size_t h = static_cast<size_t>(std::stoi(av[2]));
				if (w > 10 && h > 10 && w < 250 && h < 140)
					game.setWindowSize(w, h);
				else
				{
					std::cout << "Wrong window size" << std::endl;
					return 1;
				}
			}
		else
			game.setWindowSize(30, 50);
		game.loopTheGame();
	}
	catch (std::exception& e){
		std::cerr << "ERROR: " << e.what() << std::endl;
		return 1;
	}
	return 0;
}
