#include "SfmlLib.hpp"
#include <unistd.h>

extern "C"
{
    IWindow* create()
    {
        return new SFMLWindow;
    }
}

ResponseType SFMLWindow::getResponse()
{
    sf::Event event;
    window_->pollEvent(event);
    switch (event.type)
    {
    case sf::Event::Closed:
        return playerPressedEscape;
    case sf::Event::KeyPressed:
        switch (event.key.code)
        {
        case sf::Keyboard::Num3:
            return toNcurses;
            case sf::Keyboard::Num4:
            return changeGameMode;
        case sf::Keyboard::Escape:
            return playerPressedEscape;
        case sf::Keyboard::Num1:
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

void SFMLWindow::draw(GameField const& gameState, size_t score, size_t speed, GameMode mode)
{
    mode_ = mode;
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
         sf::VideoMode((width_ + 2) * zoomFactor_, (height_ + 2) * zoomFactor_), "SFML Nibbler");
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
    if (!font.loadFromFile("NibblerThirdParties/TextFonts/Roboto-Bold.ttf"))
        throw std::runtime_error("No font found");

    text.setFont(font);
    text.setCharacterSize(55);

    text.setString("Score: " + std::to_string((int)score_));
    text.setFillColor(sf::Color::Green);
    text.setPosition((width_ / 2) * zoomFactor_ - 60, (height_ / 2) * zoomFactor_ + 40);
    window_->draw(text);

    text.setString("Game over");
    text.setFillColor(sf::Color::Red);
    text.setPosition((width_ / 2) * zoomFactor_ - 100, (height_ / 2) * zoomFactor_ - 50);
    window_->draw(text);

    window_->display();
}

void SFMLWindow::gameStateToPixels(GameField const& gameState)
{
    for (size_t i = 0; i < height_; ++i){
        for (size_t j = 0; j < width_; ++j){
            sf::RectangleShape rectangle(sf::Vector2f(100, 100));
            rectangle.setPosition(sf::Vector2f((j + 1) * zoomFactor_, (i + 1) * zoomFactor_));
            rectangle.setFillColor(sf::Color(79, 132, 196));
            window_->draw(rectangle);
            if (gameState[i][j] != empty)
            {
                sf::CircleShape circle(15);
                circle.setPosition(sf::Vector2f((j + 1) * zoomFactor_, (i + 1) * zoomFactor_));
                setColor_[gameState[i][j]](circle);
                window_->draw(circle);
            }
        }
    }
    drawBorders();
}

void SFMLWindow::drawBorders()
{
    const sf::Color veryNiceColor(95, 158, 160);

    sf::RectangleShape left(sf::Vector2f(zoomFactor_, (height_ + 1) * zoomFactor_));
    left.setPosition(sf::Vector2f(0, 0));
    left.setFillColor(veryNiceColor);
    window_->draw(left);

    sf::RectangleShape right(sf::Vector2f(zoomFactor_, (height_ + 2) * zoomFactor_));
    right.setPosition(sf::Vector2f((width_ + 1) * zoomFactor_, 0));
    right.setFillColor(veryNiceColor);
    window_->draw(right);

    sf::RectangleShape top(sf::Vector2f((width_ + 1) * zoomFactor_, zoomFactor_));
    top.setPosition(sf::Vector2f(0, 0));
    top.setFillColor(veryNiceColor);
    window_->draw(top);

    sf::RectangleShape bottom(sf::Vector2f((width_ + 1) * zoomFactor_, zoomFactor_));
    bottom.setPosition(sf::Vector2f(0, (height_ + 1) * zoomFactor_));
    bottom.setFillColor(veryNiceColor);
    window_->draw(bottom);

    sf::Font font;
    if (!font.loadFromFile("NibblerThirdParties/TextFonts/Roboto-Bold.ttf"))
        throw std::runtime_error("No font found");
    sf::Text text;
    text.setFont(font);
    text.setCharacterSize(20);
    text.setString("Score: " + std::to_string((int)score_));
    text.setFillColor(sf::Color::Red);
    text.setPosition(zoomFactor_, (height_ + 1) * zoomFactor_);
    window_->draw(text);
    text.setPosition(width_ * zoomFactor_ - 3 * zoomFactor_,
        (height_ + 1) * zoomFactor_);
    text.setString("Speed: " + std::to_string((int)speed_));
    window_->draw(text);
}

auto SFMLWindow::initColorFunctionsArray() const
    -> ColorFunctionsArray
{
    return{{
        [](sf::CircleShape&){},
        [this](sf::CircleShape& circle){ circle.setFillColor(defineColor(127, 255, 212)); },
        [this](sf::CircleShape& circle){ circle.setFillColor(defineColor(64, 224, 208)); },
        [this](sf::CircleShape& circle){ circle.setFillColor(defineColor(255, 105, 180)); },
        [](sf::CircleShape& circle){ circle.setFillColor(sf::Color(rc(), rc(), rc())); },
        [](sf::CircleShape& circle){ circle.setFillColor(sf::Color(255, 0, 0)); },
        [this](sf::CircleShape& circle){ circle.setFillColor(defineColor(0, 255, 0)); }
    }};
}
