#include "DummyLib.hpp"
#include <iostream>
#include <vector>

extern "C"
{
	responseType getResponse()
	{
		std::cout << "getResponse of the dummy dll called" << std::endl;
		return noResponse;
	}
	void drow(std::vector<std::vector<size_t>> const& gameState)
	{
		std::cout << "Drow of the dummy dll called" << std::endl;
		for (const auto& line : gameState){
			for (const auto cell : line)
				std::cout << cell;
			std::cout << std::endl;
		}
	}
}
