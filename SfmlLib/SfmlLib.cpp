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
	sf::Event event;
	while (window_->pollEvent(event))
    {
        switch (event.type)
        {
        case sf::Event::Closed:
        	window_->close();
            return endGame;
            break;

        case sf::Event::KeyPressed:
            switch (event.key.code)
            {
            case sf::Keyboard::Num2:
            	window_->close();
                return toNcurses;
                break;
            case sf::Keyboard::Num1:
            	window_->close();
                return toSDL;
                break;
            case sf::Keyboard::Down:
            	return down;
            	break;
            case sf::Keyboard::Up:
            	return up;
            	break;
            case sf::Keyboard::Right:
            	return right;
            	break;
            case sf::Keyboard::Left:
            	return left;
            	break;
            default:
					break;
            }
        default:
        	return noResponse;
				break;
        }
    }
	return noResponse;
}

void SFMLWindow::draw(std::vector<std::vector<size_t>> const& gameState)
{
	window_->clear();
	gameStateToPixels(gameState);
	window_->display();
}

void SFMLWindow::openWindow(size_t width, size_t height)
{
	width_ = width * 10 + 10;
	height_ = height * 10 + 10;
	std::cout << "width_ = " << width_<< "height_= "<< height_ << std::endl;
	window_ = new sf::RenderWindow(sf::VideoMode(width_, height_), "SFML Nibbler");
	window_->setActive(true);
}


void SFMLWindow::gameStateToPixels(std::vector<std::vector<size_t>> const& gameState)
{
	for (size_t i = 0; i < height_ - 10; i += 10)
	{
		for (size_t j = 0; j < width_ - 10; j += 10)
		{
			sf::CircleShape circle(5);
    		circle.setPosition(sf::Vector2f(j + 10, i + 10));
			if (gameState[i / 10][j / 10] == 0)
			{
				sf::RectangleShape rectangle(sf::Vector2f(100, 100));
				rectangle.setPosition(sf::Vector2f(j + 10, i + 10));
				rectangle.setFillColor(sf::Color(79, 132, 196));
				window_->draw(rectangle);
			}
			else
			{
				if (gameState[i / 10][j / 10] == 1)
					circle.setFillColor(sf::Color(127, 255, 212));
				else if (gameState[i / 10][j / 10] == 2)
					circle.setFillColor(sf::Color(64, 224, 208));
				else if (gameState[i / 10][j / 10] == 3)
					circle.setFillColor(sf::Color(255, 105, 180));
				else if (gameState[i / 10][j / 10] == 4)
					circle.setFillColor(sf::Color(248, 14, 50));
				window_->draw(circle);
			}	
		}
	}
	drawBorders();
	usleep(100000);
}

void SFMLWindow::drawBorders()
{
	sf::RectangleShape left(sf::Vector2f(10, height_));
	left.setPosition(sf::Vector2f(0, 0));
	left.setFillColor(sf::Color(95, 158, 160));
	window_->draw(left);
	sf::RectangleShape right(sf::Vector2f(10, height_));
	right.setPosition(sf::Vector2f(width_ - 10, 0));
	right.setFillColor(sf::Color(95, 158, 160));
	window_->draw(right);
	sf::RectangleShape top(sf::Vector2f(width_, 10));
	top.setPosition(sf::Vector2f(0, 0));
	top.setFillColor(sf::Color(95, 158, 160));
	window_->draw(top);
	sf::RectangleShape bottom(sf::Vector2f(10, height_));
	bottom.setPosition(sf::Vector2f(0, height_ - 20));
	bottom.setFillColor(sf::Color(95, 158, 160));
	window_->draw(bottom);
}

bool SFMLWindow::isPaused()
{
	return paused_;
}

void SFMLWindow::closeWindow()
{
	window_->close();
	//window_->~RenderWindow();
}
