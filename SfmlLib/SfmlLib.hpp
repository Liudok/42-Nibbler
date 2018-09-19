#pragma once

#include <Nibbler.hpp>
#include <functional>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>
#include <array>

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
        void             draw(GameField const&, size_t score, size_t speed, GameMode) override;
        void             openWindow(size_t width, size_t height) override;
        void             closeWindow() override;
        void             showGameOver() override;

    private:

        void              gameStateToPixels(GameField const&);
        void              drawBorders();
        bool              isPaused();

        const size_t      zoomFactor_ = defaultZoomFaftor * 2;
        size_t            width_ = defaultWidth;
        size_t            height_ = defaultHeight;
        double            score_ = defaultScore;
        size_t            speed_ = defaultSpeed;
        GameMode          mode_ = defaultGameMode;
        std::unique_ptr<sf::RenderWindow> window_;

        inline static size_t rc()
        { return rand() % colorSpectrum; }

        inline sf::Color defineColor(size_t r, size_t g, size_t b) const
        { return (mode_ == rasta) ? sf::Color(rc(), rc(), rc()) : sf::Color(r, g, b); }

        using ColorFunctionsArray =
            std::array<std::function<void(sf::CircleShape&)>,nbGameFieldCellTypes>;
        ColorFunctionsArray setColor_ = initColorFunctionsArray();

        ColorFunctionsArray initColorFunctionsArray() const;

};
