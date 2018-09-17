#pragma once

#include <IWindow.hpp>

extern "C"
{
    IWindow* create();
}

namespace sf
{
    class RenderWindow;
}

class SFMLWindow : public IWindow
{
    public:

        responseType     getResponse() override;
        void             draw(gameField const&, size_t score, size_t speed) override;
        void             openWindow(size_t width, size_t height) override;
        void             closeWindow() override;
        void             showGameOver() override;
        ~SFMLWindow();

    private:

        void              gameStateToPixels(gameField const&);
        void              drawBorders();
        bool              isPaused();

        const size_t      zoomFactor_ = 30;
        size_t            width_ = 0;
        size_t            height_ = 0;
        size_t            score_ = 0;
        size_t            speed_ = 0;
        std::unique_ptr<sf::RenderWindow> window_;

};
