#pragma once

#include <IWindow.hpp>
#include <SDL.h>
#include <SDL_ttf.h>
#include <functional>

extern "C"
{
    IWindow* create();
}

class SDLWindow : public IWindow
{
	public:

        SDLWindow() = default;
        SDLWindow(SDLWindow const&) = delete;
        SDLWindow& operator=(SDLWindow const&) = delete;
        ~SDLWindow() = default;

        ResponseType    getResponse() override;
        void            draw(GameField const&, size_t score, size_t speed, GameMode) override;
        void            openWindow(size_t width, size_t height) override;
        void            closeWindow() override;
        void            showGameOver() override;

    private:

        void            gameStateToPixels(GameField const& gameState);
        void            drawBorders();
        SDL_Rect        makeRect(size_t x, size_t y, size_t h, size_t w);
        void            showText(const char *text, size_t x, size_t y,
                            SDL_Color color, size_t fontSize);
        size_t          zoomFont(size_t fontSize);

        const size_t    zoomFactor_ = 14;
        size_t          width_ = 0;
        size_t          height_ = 0;
        SDL_Window      *window_ = nullptr;
        SDL_Renderer    *renderer_ = nullptr;
        size_t          score_ = 0;
        size_t          speed_ = 0;
        GameMode        mode_ = classic;

        static constexpr size_t colorSpectrum = 255;

        inline static size_t rc()
        { return rand() % colorSpectrum; }

        inline void setColor(size_t r, size_t g, size_t b)
        { (mode_ == rasta) ?
            SDL_SetRenderDrawColor(renderer_, rc(), rc(), rc(), colorSpectrum) :
                SDL_SetRenderDrawColor(renderer_, r, g, b, colorSpectrum); }

        std::function<void()> setColor_[nbGameFieldCellTypes] = {
            [this](){ SDL_SetRenderDrawColor(renderer_, 79, 132, 196,colorSpectrum); },
            [this](){ setColor(127, 255, 212); },
            [this](){ setColor(64, 224, 208); },
            [this](){ setColor(255, 105, 180); },
            [this](){ SDL_SetRenderDrawColor(renderer_, rc(), rc(), rc(), colorSpectrum); },
            [this](){ SDL_SetRenderDrawColor(renderer_, 255, 0, 0, colorSpectrum); },
            [this](){ setColor(11, 111, 144); }
        };

};
