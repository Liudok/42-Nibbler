#pragma once

#include <IWindow.hpp>
#include <functional>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

extern "C"
{
    IWindow* create();
}

class SFMLWindow : public IWindow
{
    public:

        SFMLWindow() = default;
        SFMLWindow(SFMLWindow const&) = delete;
        SFMLWindow& operator=(SFMLWindow const&) = delete;
        ~SFMLWindow() = default;

        ResponseType     getResponse() override;
        void             draw(GameField const&, size_t score, size_t speed) override;
        void             openWindow(size_t width, size_t height) override;
        void             closeWindow() override;
        void             showGameOver() override;

    private:

        void              gameStateToPixels(GameField const&);
        void              drawBorders();
        bool              isPaused();

        const size_t      zoomFactor_ = 30;
        size_t            width_ = 0;
        size_t            height_ = 0;
        size_t            score_ = 0;
        size_t            speed_ = 0;
        std::unique_ptr<sf::RenderWindow> window_;

        std::function<void(sf::CircleShape&)> setColor_[nbGameFieldCellTypes] = {
            [](sf::CircleShape&){},
            [](sf::CircleShape& circle){ circle.setFillColor(sf::Color(127, 255, 212)); },
            [](sf::CircleShape& circle){ circle.setFillColor(sf::Color(64, 224, 208)); },
            [](sf::CircleShape& circle){ circle.setFillColor(sf::Color(255, 105, 180)); },
            [](sf::CircleShape& circle){ circle.setFillColor(
                sf::Color(rand() % 255, rand() % 255, rand() % 255)); },
            [](sf::CircleShape& circle){ circle.setFillColor(sf::Color::Red); },
            [](sf::CircleShape& circle){ circle.setFillColor(sf::Color::Green); }
        };

};
