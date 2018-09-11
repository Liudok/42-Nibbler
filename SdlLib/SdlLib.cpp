#include "SdlLib.hpp"
#include <iostream>
#include <unistd.h>
extern "C"

{
	IWindow* create()
	{
		return new SDLWindow;
	}
}

responseType SDLWindow::getResponse()
{
while (SDL_PollEvent(&event_))
	{
		if (event_.type == SDL_QUIT || (event_.type == SDL_KEYDOWN && event_.key.keysym.sym == SDLK_ESCAPE))
			return endGame;
		else if (event_.type == SDL_KEYDOWN )
		{
			if (isPaused() && event_.key.keysym.sym != SDLK_SPACE)
				return pauseGame;
			else
			{
				paused_ = false;
				if (event_.key.keysym.sym == SDLK_UP || event_.key.keysym.sym == SDLK_w)
					return up;
				else if (event_.key.keysym.sym == SDLK_DOWN || event_.key.keysym.sym == SDLK_s)
					return down;
				else if (event_.key.keysym.sym == SDLK_LEFT || event_.key.keysym.sym == SDLK_a)
					return left;
				else if (event_.key.keysym.sym == SDLK_RIGHT || event_.key.keysym.sym == SDLK_d)
					return right;
				else if (event_.key.keysym.sym == SDLK_SPACE)
				{
					paused_ = true;
					return pauseGame;
				}
				else if (event_.key.keysym.sym == SDLK_2)
					return toNcurses;
				else if (event_.key.keysym.sym == SDLK_1)
					return toSFML;
			}
		}
	}
	if (isPaused() && event_.key.keysym.sym != SDLK_SPACE)
				return pauseGame;
	return noResponse;
}

void SDLWindow::draw(std::vector<std::vector<size_t>> const& gameState)
{
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
}


void SDLWindow::gameStateToPixels(std::vector<std::vector<size_t>> const& gameState)
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
				SDL_SetRenderDrawColor( renderer_, 79, 132, 196, 255 );
			}
			else
			{
				if (gameState[i][j] == 1)
					SDL_SetRenderDrawColor( renderer_, 127, 255, 212, 255 );
				else if (gameState[i][j] == 2)
					SDL_SetRenderDrawColor( renderer_, 64, 224, 208, 255 );
				else if (gameState[i][j] == 3)
					SDL_SetRenderDrawColor( renderer_, 255, 105, 180, 255 );
				else if (gameState[i][j] == 4)
					SDL_SetRenderDrawColor( renderer_, 248, 14, 50, 255 );
			}
			SDL_RenderFillRect( renderer_, &rectangle );
		}
	}
	drawBorders();
	usleep( 100000 );
	SDL_RenderPresent( renderer_ );
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
}

bool SDLWindow::isPaused()
{
	return paused_;
}

void SDLWindow::setScore(size_t score)
{
	score_ = score;
}

void SDLWindow::setSpeed(size_t speed)
{
	speed_ = speed;
}

SDL_Rect SDLWindow::makeRect(size_t x, size_t y, size_t h, size_t w)
{
	SDL_Rect	rect;

	rect.x = x * 2;
	rect.y = y * 2;
	rect.h = h * 2;
	rect.w = w * 2;
	return (rect);
}

void SDLWindow::showGameOver()
{

	SDL_Surface				*surface;
	static const SDL_Color	color = {199, 50, 176, 0};
	TTF_Font				*font;

	TTF_Init();
	font = TTF_OpenFont("Roboto/Roboto-Light.ttf", 20);
	if (font == NULL)
		return ;
	TTF_SetFontStyle(font, TTF_STYLE_BOLD);
	surface = TTF_RenderUTF8_Blended(font, "Game over", color);
	SDL_Rect	rect = makeRect((width_ / 2) * 4 - 10, (height_ / 2) * 4 - 20, surface->h / 2, surface->w / 2);
	SDL_Texture		*texture = SDL_CreateTextureFromSurface(renderer_, surface);
	SDL_RenderCopy(renderer_, texture, NULL, &rect);
	SDL_FreeSurface(surface);
	TTF_CloseFont(font);
	TTF_Quit();
	SDL_RenderPresent( renderer_ );
}

void SDLWindow::closeWindow()
{
	SDL_DestroyRenderer(renderer_);
	SDL_DestroyWindow(window_);
	SDL_Quit();
}
