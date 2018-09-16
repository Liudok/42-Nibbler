#include "SfmlLib.hpp"
#include <unistd.h>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

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
            case sf::Keyboard::Num3:
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

            case sf::Keyboard::Space:
                return pauseContinue;

            default:
                break;
            }
        default:
            return noResponse;
        }
    }
    return noResponse;
}

void SFMLWindow::draw(field const& gameState, size_t score, size_t speed)
{
    score_ = score;
    speed_ = speed;
    window_->clear();
    gameStateToPixels(gameState);
    window_->display();
}

void SFMLWindow::openWindow(size_t width, size_t height)
{
    width_ = width;
    height_ = height;
    window_ = std::make_unique<sf::RenderWindow>(
        sf::VideoMode(width_ * 30 + 30, height_ * 30 + 30), "SFML Nibbler");
    window_->setActive(true);
}

void SFMLWindow::closeWindow()
{
    window_->close();
}

void SFMLWindow::showGameOver()
{
    sf::Text text;
    sf::Font font;
    if (!font.loadFromFile("../NibblerThirdParties/TextFonts/Roboto-Bold.ttf"))
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

void SFMLWindow::gameStateToPixels(field const& gameState)
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
                if (gameState[i][j] == body)
                    circle.setFillColor(sf::Color(127, 255, 212));
                else if (gameState[i][j] == collision)
                	circle.setFillColor(sf::Color::Red);
                else if (gameState[i][j] == 2)
                    circle.setFillColor(sf::Color(64, 224, 208));
                else if (gameState[i][j] == food)
                    circle.setFillColor(sf::Color(255, 105, 180));
                else if (gameState[i][j] == obstacle)
                    circle.setFillColor(sf::Color(0, 0, 0));
                else if (gameState[i][j] == collision)
                    circle.setFillColor(sf::Color(248, 14, 50));
                window_->draw(circle);
            }   
        }
    }
    drawBorders();
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
    if (!font.loadFromFile("../NibblerThirdParties/TextFonts/Roboto-Bold.ttf"))
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

SFMLWindow::~SFMLWindow()
{
    
}
