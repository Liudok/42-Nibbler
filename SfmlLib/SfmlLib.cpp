#include "SfmlLib.hpp"
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
        	closeWindow();
            return endGame;
        case sf::Event::KeyPressed:
            switch (event.key.code)
            {
            case sf::Keyboard::Num2:
            	closeWindow();
                return toNcurses;
   
            case sf::Keyboard::Escape:
            	closeWindow();
                return endGame;
   
            case sf::Keyboard::Num1:
            	closeWindow();
                return toSDL;
   
            case sf::Keyboard::Down:
            	return down;

            case sf::Keyboard::Up:
            	return up;

            case sf::Keyboard::Right:
            	return right;

            case sf::Keyboard::Left:
            	return left;

            default:
				break;
            }
        default:
        	return noResponse;
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
	width_ = width;
	height_ = height;
	window_ = new sf::RenderWindow(sf::VideoMode(width_ * 30 + 30, height_ * 30 + 30), "SFML Nibbler");
	window_->setActive(true);
}

void SFMLWindow::gameStateToPixels(std::vector<std::vector<size_t>> const& gameState)
{
	for (size_t i = 0; i < height_; ++i)
	{
		for (size_t j = 0; j < width_; ++j)
		{
			sf::RectangleShape rectangle(sf::Vector2f(100, 100));
			rectangle.setPosition(sf::Vector2f(j * 30 + 30, i * 30 + 30));
			rectangle.setFillColor(sf::Color(79, 132, 196));
			window_->draw(rectangle);
			sf::CircleShape circle(15);
    		circle.setPosition(sf::Vector2f(j * 30 + 30, i * 30 + 30));
			if (gameState[i][j] != 0)
			{
				if (gameState[i][j] == 1)
					circle.setFillColor(sf::Color(127, 255, 212));
				else if (gameState[i][j] == 2)
					circle.setFillColor(sf::Color(64, 224, 208));
				else if (gameState[i][j] == 3)
					circle.setFillColor(sf::Color(255, 105, 180));
				else if (gameState[i][j] == 4)
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
	sf::RectangleShape left(sf::Vector2f(30, height_ * 30));
	left.setPosition(sf::Vector2f(0, 0));
	left.setFillColor(sf::Color(95, 158, 160));
	window_->draw(left);
	sf::RectangleShape right(sf::Vector2f(30, height_ * 30 + 30));
	right.setPosition(sf::Vector2f(width_ * 30, 0));
	right.setFillColor(sf::Color(95, 158, 160));
	window_->draw(right);
	sf::RectangleShape top(sf::Vector2f(width_ * 30, 30));
	top.setPosition(sf::Vector2f(0, 0));
	top.setFillColor(sf::Color(95, 158, 160));
	window_->draw(top);
	sf::RectangleShape bottom(sf::Vector2f(width_ * 30 + 30, 30));
	bottom.setPosition(sf::Vector2f(0, height_ * 30));
	bottom.setFillColor(sf::Color(95, 158, 160));
	window_->draw(bottom);
	sf::Font font;
	if (!font.loadFromFile("Roboto/Roboto-Bold.ttf"))
		throw std::runtime_error("No font found");
	sf::Text text;
	text.setFont(font);
	text.setCharacterSize(20);
	text.setString("Score: " + std::to_string((int)score_));
	text.setFillColor(sf::Color::Red);
	text.setPosition(10,height_ * 30);
	window_->draw(text);
	text.setPosition(width_ * 30 - 100, height_ * 30);
	text.setString("Speed: " + std::to_string((int)speed_));
	window_->draw(text);
}

void SFMLWindow::setScore(size_t score)
{
	score_ = score;
}

void SFMLWindow::setSpeed(size_t speed)
{
	speed_ = speed;
}

bool SFMLWindow::isPaused()
{
	return paused_;
}

void SFMLWindow::showGameOver()
{
	sf::Text text;
	sf::Font font;
	if (!font.loadFromFile("Roboto/Roboto-Bold.ttf"))
		throw std::runtime_error("No font found");
	text.setFont(font);
	text.setCharacterSize(55);
	text.setString("Score: " + std::to_string((int)score_));
	text.setFillColor(sf::Color::Green);
	text.setPosition((width_ / 2) * 30 - 60, (height_ / 2) * 30 - 50);
	window_->draw(text);
	text.setString("Game over");
	text.setFillColor(sf::Color::Red);
	text.setPosition((width_ / 2) * 30 - 100, (height_ / 2) * 30 + 40);
	window_->draw(text);
	window_->display();
}

void SFMLWindow::closeWindow()
{
	window_->close();
}

SFMLWindow::~SFMLWindow()
{

}
