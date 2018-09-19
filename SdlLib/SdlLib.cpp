#include "SdlLib.hpp"
#include <unistd.h>
#include <string>
#include <cmath>

extern "C"
{
    IWindow* create()
    {
        return new SDLWindow;
    }
}

ResponseType SDLWindow::getResponse()
{
    SDL_Event event;
    SDL_PollEvent(&event);
    if (event.type == SDL_QUIT || (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE))
        return playerPressedEscape;
    else if (event.type == SDL_KEYDOWN)
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
        else if (event.key.keysym.sym == SDLK_4)
            return changeGameMode;
        else if (event.key.keysym.sym == SDLK_3)
            return toGlfw;
        else if (event.key.keysym.sym == SDLK_2)
            return toSFML;
    }
    return noResponse;
}

void SDLWindow::draw(GameField const& gameState, size_t score, size_t speed, GameMode mode)
{
    mode_ = mode;
    score_ = score;
    speed_ = speed;
    gameStateToPixels(gameState);
    SDL_RenderPresent(renderer_);
}

void SDLWindow::openWindow(size_t width, size_t height)
{
    SDL_Init(SDL_INIT_VIDEO);
    TTF_Init();
    width_ = width;
    height_ = height;

    window_ = SDL_CreateWindow(
        "SDL Nibbler", 
        SDL_WINDOWPOS_CENTERED,
        SDL_WINDOWPOS_CENTERED,
        width_ * zoomFactor_ + 2 * zoomFactor_,
        height_ * zoomFactor_ + 2 * zoomFactor_, 0);

    renderer_ = SDL_CreateRenderer(window_, -1, 0);
}

void SDLWindow::closeWindow()
{
    SDL_DestroyRenderer(renderer_);
    SDL_DestroyWindow(window_);
    TTF_Quit();
    SDL_Quit();
}

void SDLWindow::showGameOver()
{
    const auto xShift = width_ * zoomFactor_ / 5.9;
    const auto yShift = height_ * zoomFactor_ / 4.7;
    const auto scoreDownShift = 1.3;
    const size_t gameOverFontSize = 20;
    {
        const SDL_Color color = {199, 50, 176, 0};
        showText("Game over", xShift, yShift, color,
        zoomFont(gameOverFontSize));
    }
    {
        const SDL_Color color = {59, 150, 116, 0};
        std::string score("Score: " + std::to_string(score_));
        const char *text = score.c_str();
        showText(text, xShift, yShift * scoreDownShift,
            color, zoomFont(gameOverFontSize));
    }
    SDL_RenderPresent(renderer_);
}

void SDLWindow::gameStateToPixels(GameField const& gameState)
{
    for (size_t i = 0; i < height_; ++i){
        for (size_t j = 0; j < width_; ++j){
            SDL_Rect rectangle;
            rectangle.x = (j + 1) * zoomFactor_;
            rectangle.y = (i + 1) * zoomFactor_;
            rectangle.w = zoomFactor_;
            rectangle.h = zoomFactor_;
            setColor_[gameState[i][j]]();
            SDL_RenderFillRect(renderer_, &rectangle);
        }
    }
   drawBorders();
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
    top.w = (width_ + 1) * zoomFactor_;
    top.h = zoomFactor_;
    SDL_RenderFillRect(renderer_, &top);
    
    bottom.x = 0;
    bottom.y = (height_ + 1) * zoomFactor_;
    bottom.w = (width_ + 2) * zoomFactor_;
    bottom.h = zoomFactor_;
    SDL_RenderFillRect(renderer_, &bottom);
    
    right.x = (width_ + 1) * zoomFactor_;
    right.y = 0;
    right.w = zoomFactor_;
    right.h = (height_ + 1) * zoomFactor_;
    SDL_RenderFillRect(renderer_, &right);
    
    left.x = 0;
    left.y = 0;
    left.w = zoomFactor_;
    left.h = (height_ + 1) * zoomFactor_;
    SDL_RenderFillRect(renderer_, &left);

    const auto textFontSize = 14;
    const SDL_Color veryNiceColor{199, 50, 176, 0};
    const auto score = "score: " + std::to_string(score_);
    showText(score.c_str(), 0, 0, veryNiceColor, textFontSize);
    const auto speed = "speed: " + std::to_string(
        static_cast<size_t>(speed_));
    showText(speed.c_str(), (width_ / 2.29) * zoomFactor_, 0,
        veryNiceColor, textFontSize);
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

void  SDLWindow::showText(const char *text, size_t x, size_t y, SDL_Color color, size_t fontSize)
{
    auto font = TTF_OpenFont("NibblerThirdParties/TextFonts/Roboto-Light.ttf", fontSize);
    if (!font){
        closeWindow();
        throw std::runtime_error("No font found.");
    }
    TTF_SetFontStyle(font, TTF_STYLE_BOLD);
    auto surface = TTF_RenderText_Solid(font, text, color);

    auto rect = makeRect(x, y, surface->h / 2, surface->w / 2);
    auto texture = SDL_CreateTextureFromSurface(renderer_, surface);
    SDL_RenderCopy(renderer_, texture, NULL, &rect);

    SDL_DestroyTexture(texture);
    SDL_FreeSurface(surface);
    

    TTF_CloseFont(font);
}

size_t SDLWindow::zoomFont(size_t fontSize)
{
    return std::pow(fontSize * width_ * height_ * zoomFactor_, 1.0 / 3.5);
}

auto SDLWindow::initColorFunctionsArray()
    -> ColorFunctionsArray
{
    return{{
        [this](){ SDL_SetRenderDrawColor(renderer_, 79, 132, 196,colorSpectrum); },
        [this](){ setColor(127, 255, 212); },
        [this](){ setColor(64, 224, 208); },
        [this](){ setColor(255, 105, 180); },
        [this](){ SDL_SetRenderDrawColor(renderer_, rc(), rc(), rc(), colorSpectrum); },
        [this](){ SDL_SetRenderDrawColor(renderer_, 255, 0, 0, colorSpectrum); },
        [this](){ setColor(11, 111, 144); }
    }};
}
