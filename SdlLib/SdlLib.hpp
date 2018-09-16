#pragma once

#include <IWindow.hpp>
#include <SDL.h>
#include <SDL_ttf.h>

extern "C"
{
    IWindow* create();
}

class SDLWindow : public IWindow
{
	public:

        responseType    getResponse() override;
        void            draw(field const&, size_t score, size_t speed) override;
        void            openWindow(size_t width, size_t height) override;
        void            closeWindow() override;
        void            showGameOver() override;
        ~SDLWindow();

    private:

        void            gameStateToPixels(std::vector<std::vector<size_t>> const& gameState);
        void            drawBorders();
        SDL_Rect        makeRect(size_t x, size_t y, size_t h, size_t w);
        void            textureFromText(const char* text, size_t x, size_t y, SDL_Color color);
        void            showText(const char *text, size_t x, size_t y, SDL_Color color);

        size_t          width_ = 0;
        size_t          height_ = 0;
        SDL_Window      *window_ = nullptr;
        SDL_Renderer    *renderer_ = nullptr;
        SDL_Surface     *scoreSurface_ = nullptr;
        SDL_Rect        scoreRect_;
        SDL_Texture     *scoreTexture_ = nullptr;
        TTF_Font        *font_ = nullptr;
        size_t          score_ = 0;
};
