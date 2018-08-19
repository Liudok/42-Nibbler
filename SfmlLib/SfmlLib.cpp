#include "SfmlLib.hpp"
#include <iostream>
#include <unistd.h>
extern "C"
{
	IWindow* create()
	{
		return new SFMLWindow;
	}
}

responseType SFMLWindow::getResponse()
{


// while (window_->isOpen())
//     {
        sf::Event e;
        while (window_->pollEvent(e))
        {
            if (e.type == sf::Event::Closed)
            {
                window_->close();
            }
        }

        
    // }
// while (SFML_PollEvent(&event_))
// 	{
	// 	if (event_.type == SFML_QUIT || (event_.type == SFML_KEYDOWN && event_.key.keysym.sym == SFMLK_ESCAPE))
	// 		return endGame;
	// 	else if (event_.type == SFML_KEYDOWN )
	// 	{
	// 		if (isPaused() && event_.key.keysym.sym != SFMLK_SPACE)
	// 			return pauseGame;
	// 		else
	// 		{
	// 			paused_ = false;
	// 		if (event_.key.keysym.sym == SFMLK_UP || event_.key.keysym.sym == SFMLK_w)
	// 			return up;
	// 		else if (event_.key.keysym.sym == SFMLK_DOWN || event_.key.keysym.sym == SFMLK_s)
	// 			return down;
	// 		else if (event_.key.keysym.sym == SFMLK_LEFT || event_.key.keysym.sym == SFMLK_a)
	// 			return left;
	// 		else if (event_.key.keysym.sym == SFMLK_RIGHT || event_.key.keysym.sym == SFMLK_d)
	// 			return right;
	// 		else if (event_.key.keysym.sym == SFMLK_SPACE)
	// 			{
	// 				paused_ = true;
	// 				return pauseGame;
	// 			}
	// 		else if (event_.key.keysym.sym == SFMLK_z)
	// 			return toNcurses;
	// 		else if (event_.key.keysym.sym == SFMLK_x)
	// 			return toDummy;
	// 		}
	// 	}
	// }
	// if (isPaused() && event_.key.keysym.sym != SFMLK_SPACE)
	// 			return pauseGame;
	return noResponse;
}

void SFMLWindow::draw(std::vector<std::vector<size_t>> const& gameState)
{
	 gameStateToPixels(gameState);
	 window_->clear();
        window_->display();
	// SFML_RenderClear(renderer_);
	// SFML_RenderCopy(renderer_, canvas_, NULL, NULL);
}

void SFMLWindow::openWindow(size_t width, size_t height)
{
	width_ = width * 10 + 10;
	height_ = height * 10 + 10;
	std::cout << "width_ = " << width_<< "height_= "<<height_ << std::endl;
	window_ = new sf::RenderWindow(sf::VideoMode(width_, height_), "Nibbler - SFML");
	//sf::RenderWindow window(sf::VideoMode(640, 480), "Window");

    
	// SFML_Init(SFML_INIT_EVERYTHING);

	// window_ = SFML_CreateWindow(
	// 	"Nibbler", 
	// 	SFML_WINDOWPOS_CENTERED,
	// 	SFML_WINDOWPOS_CENTERED,
	// 	width_,
	// 	height_,
	// 	SFML_WINDOW_OPENGL |SFML_WINDOW_INPUT_GRABBED |
	// 									SFML_WINDOW_SHOWN);

	// renderer_ = SFML_CreateRenderer(
	// 	window_,
	// 	-1,
	// 	SFML_RENDERER_ACCELERATED);

}


void SFMLWindow::gameStateToPixels(std::vector<std::vector<size_t>> const& gameState)
{
	for (size_t i = 0; i < height_ - 10; i += 10)
	{
		for (size_t j = 0; j < width_ - 10; j += 10)
		{
	// 		SFML_Rect rectangle;

	// 		rectangle.x = j + 10;
	// 		rectangle.y = i + 10;
	// 		rectangle.w = 10;
	// 		rectangle.h = 10;
			if (gameState[i / 10][j / 10] == 0)
			{
	// 			SFML_SetRenderDrawColor( renderer_, 79, 132, 196, 255 );
	// 			SFML_RenderFillRect(renderer_, &rectangle);
			}
	// 		else
	// 		{
	// 			if (gameState[i / 10][j / 10] == 1)
	// 				SFML_SetRenderDrawColor( renderer_, 127, 255, 212, 255 );
	// 			else if (gameState[i / 10][j / 10] == 2)
	// 				SFML_SetRenderDrawColor( renderer_, 64, 224, 208, 255 );
	// 			else if (gameState[i / 10][j / 10] == 3)
	// 				SFML_SetRenderDrawColor( renderer_, 255, 105, 180, 255 );
	// 			else if (gameState[i / 10][j / 10] == 4)
	// 				SFML_SetRenderDrawColor( renderer_, 248, 14, 50, 255 );
	// 			SFML_RenderFillRect(renderer_, &rectangle);
	// 		}
		}
	}
	// drawBorders();
	// usleep(90000);
	// SFML_RenderPresent( renderer_ );
}

void SFMLWindow::drawBorders()
{
	// SFML_Rect top;
	// SFML_Rect bottom;
	// SFML_Rect right;
	// SFML_Rect left;
	// SFML_SetRenderDrawColor(renderer_, 95, 158, 160, 255);
	
	// top.x = 0;
	// top.y = 0;
	// top.w = width_;
	// top.h = 10;
	// SFML_RenderFillRect(renderer_, &top);
	// bottom.x = 0;
	// bottom.y = height_ - 10;
	// bottom.w = width_;
	// bottom.h = 10;
	// SFML_RenderFillRect(renderer_, &bottom);
	// right.x = width_ - 10;
	// right.y = 0;
	// right.w = 10;
	// right.h = height_;
	// SFML_RenderFillRect(renderer_, &right);
	// left.x = 0;
	// left.y = 0;
	// left.w = 10;
	// left.h = height_;
	// SFML_RenderFillRect(renderer_, &left);
}

bool SFMLWindow::isPaused()
{
	return paused_;
}

void SFMLWindow::closeWindow()
{
	// SFML_DestroyTexture(canvas_);
	// SFML_DestroyRenderer(renderer_);
	// SFML_DestroyWindow(window_);
	// SFML_Quit();
}
