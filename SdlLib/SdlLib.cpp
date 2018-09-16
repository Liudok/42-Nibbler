#include "SdlLib.hpp"
#include <unistd.h>
#include <string>

extern "C"
{
    IWindow* create()
    {
        return new SDLWindow;
    }
}

responseType SDLWindow::getResponse()
{
    SDL_Event event;
    while (SDL_PollEvent(&event))
    {
        if (event.type == SDL_QUIT || (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE))
            return endGame;
        else if (event.type == SDL_KEYDOWN )
        {
            if (event.key.keysym.sym == SDLK_UP || event.key.keysym.sym == SDLK_w)
                return up;
            else if (event.key.keysym.sym == SDLK_DOWN || event.key.keysym.sym == SDLK_s)
                return down;
            else if (event.key.keysym.sym == SDLK_LEFT || event.key.keysym.sym == SDLK_a)
                return left;
            else if (event.key.keysym.sym == SDLK_RIGHT || event.key.keysym.sym == SDLK_d)
                return right;
            else if (event.key.keysym.sym == SDLK_SPACE)
                return pauseContinue;
            else if (event.key.keysym.sym == SDLK_3)
                return toNcurses;
            else if (event.key.keysym.sym == SDLK_2)
                return toSFML;
        }
    }
    return noResponse;
}

void SDLWindow::draw(field const& gameState, size_t score, size_t)
{
    score_ = score;
    gameStateToPixels(gameState);
    SDL_RenderClear(renderer_);
}

void SDLWindow::openWindow(size_t width, size_t height)
{
    width_ = width;
    height_ = height;
    SDL_Init(SDL_INIT_EVERYTHING);

    window_ = SDL_CreateWindow(
        "SDL Nibbler", 
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        width_ * 10 + 10,
        height_ * 10 + 10,
        SDL_WINDOW_OPENGL |SDL_WINDOW_INPUT_GRABBED |
        SDL_WINDOW_SHOWN);

    renderer_ = SDL_CreateRenderer(
        window_,
        -1,
        SDL_RENDERER_ACCELERATED);
    SDL_RaiseWindow(window_);
    TTF_Init();
}

void SDLWindow::closeWindow()
{
    TTF_Quit();
    SDL_DestroyRenderer(renderer_);
    SDL_DestroyWindow(window_);
    SDL_Quit();
}

void SDLWindow::showGameOver()
{
    SDL_Color color = {199, 50, 176, 0};
    showText("Game over", (width_ / 2) * 4 - 5, (height_ / 2) * 4, color);
    color = {59, 150, 116, 0};
    std::string score("Score: " + std::to_string((int)score_));
    const char *text = score.c_str();
    showText(text, (width_ / 2) * 4, (height_ / 2) * 4 + 20, color);
    SDL_RenderPresent(renderer_);
}

SDLWindow::~SDLWindow()
{
    
}

void SDLWindow::gameStateToPixels(field const& gameState)
{
    for (size_t i = 0; i < height_; ++i)
    {
        for (size_t j = 0; j < width_; ++j)
        {
            SDL_Rect rectangle;

            rectangle.x = j * 10 + 10;
            rectangle.y = i * 10 + 10;
            rectangle.w = 10;
            rectangle.h = 10;
            if (gameState[i][j] == 0)
            {
                SDL_SetRenderDrawColor(renderer_, 79, 132, 196, 255);
            }
            else
            {
                if (gameState[i][j] == 1)
                    SDL_SetRenderDrawColor(renderer_, 127, 255, 212, 255);
                else if (gameState[i][j] == 2)
                    SDL_SetRenderDrawColor(renderer_, 64, 224, 208, 255);
                else if (gameState[i][j] == 3)
                    SDL_SetRenderDrawColor(renderer_, 255, 105, 180, 255);
                else if (gameState[i][j] == 4)
                    SDL_SetRenderDrawColor(renderer_, 248, 14, 50, 255);
            }
            SDL_RenderFillRect(renderer_, &rectangle);
        }
    }
    drawBorders();
    SDL_RenderPresent(renderer_);
}

void SDLWindow::drawBorders()
{
    SDL_Rect top;
    SDL_Rect bottom;
    SDL_Rect right;
    SDL_Rect left;
    SDL_SetRenderDrawColor(renderer_, 95, 158, 160, 255);

    top.x = 0;
    top.y = 0;
    top.w = width_ * 10;
    top.h = 10;
    SDL_RenderFillRect(renderer_, &top);
    bottom.x = 0;
    bottom.y = height_ * 10;
    bottom.w = width_ * 10 + 10;
    bottom.h = 10;
    SDL_RenderFillRect(renderer_, &bottom);
    right.x = width_ * 10;
    right.y = 0;
    right.w = 10;
    right.h = height_ * 10 + 10;
    SDL_RenderFillRect(renderer_, &right);
    left.x = 0;
    left.y = 0;
    left.w = 10;
    left.h = height_ * 10;
    SDL_RenderFillRect(renderer_, &left);

    const auto score = "score: " + std::to_string(score_);
    showText(score.c_str(), 2, height_ * 5 - 1, {199, 50, 176, 0});
}

SDL_Rect SDLWindow::makeRect(size_t x, size_t y, size_t h, size_t w)
{
    SDL_Rect result;
    result.x = x * 2;
    result.y = y * 2;
    result.h = h * 2;
    result.w = w * 2;
    return result;
}

void  SDLWindow::showText(const char *text, size_t x, size_t y, SDL_Color color)
{
    TTF_Font        *font = TTF_OpenFont("NibblerThirdParties/TextFonts/Roboto-Light.ttf", 11);
    if (font == nullptr) return ;
    TTF_SetFontStyle(font, TTF_STYLE_BOLD);
    auto surface = TTF_RenderUTF8_Blended(font, text, color);
    auto rect = makeRect(x, y, surface->h / 2, surface->w / 2);
    auto texture = SDL_CreateTextureFromSurface(renderer_, surface);
    SDL_RenderCopy(renderer_, texture, NULL, &rect);
    SDL_DestroyTexture(texture);
    SDL_FreeSurface(surface);
}
