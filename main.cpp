#include "LogicLib/LogicLib.hpp"
#include <iostream>

int main()
{
	try{
		LogicUnit().loopTheGame();
	}
	catch (std::exception& e){
		std::cout << e.what() << std::endl;
		return 1;
	}
	return 0;
}
