#pragma once

#include <Nibbler.hpp>
#include <SDL.h>
#include <SDL_ttf.h>
#include <functional>
#include <array>

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
        void            draw(GameField const&, size_t score, double speed, GameMode) override;
        void            openWindow(size_t width, size_t height) override;
        void            closeWindow() override;
        void            showGameOver() override;

    private:

        void            gameStateToPixels(GameField const&);
        void            drawBorders();
        SDL_Rect        makeRect(size_t x, size_t y, size_t h, size_t w);
        void            showText(const char *text, size_t x, size_t y,
                            SDL_Color color, size_t fontSize);
        size_t          zoomFont(size_t fontSize);

        const size_t    zoomFactor_ = defaultZoomFaftor;
        size_t          width_ = defaultWidth;
        size_t          height_ = defaultHeight;

        SDL_Window*     window_ = nullptr;
        SDL_Renderer*   renderer_ = nullptr;
        size_t          score_ = defaultScore;
        double          speed_ = defaultSpeed;
        GameMode        mode_ = defaultGameMode;

        inline static size_t rc()
        { return rand() % colorSpectrum; }

        inline void setColor(size_t r, size_t g, size_t b)
        { (mode_ == rasta) ?
            SDL_SetRenderDrawColor(renderer_, rc(), rc(), rc(), colorSpectrum) :
                SDL_SetRenderDrawColor(renderer_, r, g, b, colorSpectrum); }

        using ColorFunctionsArray =
            std::array<std::function<void()>, nbGameFieldCellTypes>;
        ColorFunctionsArray setColor_ = initColorFunctionsArray();

        ColorFunctionsArray initColorFunctionsArray();

};
