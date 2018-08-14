#include "SdlLib.hpp"

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
			if (event_.key.keysym.sym == SDLK_UP || event_.key.keysym.sym == SDLK_w)
				return left;
			else if (event_.key.keysym.sym == SDLK_DOWN || event_.key.keysym.sym == SDLK_s)
				return right;
			else if (event_.key.keysym.sym == SDLK_LEFT || event_.key.keysym.sym == SDLK_a)
				return up;
			else if (event_.key.keysym.sym == SDLK_RIGHT || event_.key.keysym.sym == SDLK_d)
				return down;
			else if (event_.key.keysym.sym == SDLK_z)
				return toNcurses;
			else if (event_.key.keysym.sym == SDLK_x)
				return toDummy;
		}
	}
	return noResponse;
}

void SDLWindow::draw(std::vector<std::vector<size_t>> const& gameState)
{
	SDL_SetRenderDrawColor(renderer_, 0, 0, 0, 255);
	gameStateToPixels(gameState);
	SDL_UpdateTexture(canvas_, NULL, pixels_, width_ << 2);
	SDL_RenderClear(renderer_);
	SDL_RenderCopy(renderer_, canvas_, NULL, NULL);
}

void SDLWindow::openWindow(size_t width, size_t height)
{
	width_ = width * 10;
	height_ = height * 10;
	SDL_Init(SDL_INIT_VIDEO);

	window_ = SDL_CreateWindow(
		"Nibbler", 
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		width_,
		height_,
		0);

	renderer_ = SDL_CreateRenderer(
		window_,
		-1,
		SDL_RENDERER_ACCELERATED);

	canvas_ = SDL_CreateTexture(
		renderer_,
		SDL_PIXELFORMAT_ARGB8888,
		SDL_TEXTUREACCESS_TARGET,
		width_,
		height_);
	SDL_SetRenderDrawColor(renderer_, 0, 255, 0, 255);
	if (pixels_ != nullptr)
		delete pixels_;
	pixels_ = (int*)malloc(sizeof(int) * width_ * height_);
	bzero(pixels_, sizeof(int) * width_ * height_);

}


void SDLWindow::gameStateToPixels(std::vector<std::vector<size_t>> const& gameState)
{
	const size_t nbRows = gameState.size() * 10;
	const size_t nbColumns = gameState[0].size() * 10;
	for (size_t i = 0; i < nbRows; ++i){
		for (size_t j = 0; j < nbColumns; ++j)
		{
			SDL_Rect rectangle;

			rectangle.x = i;
			rectangle.y = j;
			rectangle.w = 10;
			rectangle.h = 10;
			SDL_RenderFillRect(renderer_, &rectangle);
			if (gameState[i / 10][j / 10] == 0)
			{
				SDL_SetRenderDrawColor(renderer_, 214, 48, 250, 255);
				SDL_RenderDrawRect( renderer_, &rectangle );	
			}
			else
			{
				SDL_SetRenderDrawColor( renderer_, 0, 255,0, 255 );
				SDL_RenderDrawRect( renderer_, &rectangle );
			}
		}
	}
	drawBorders();
	SDL_RenderPresent( renderer_ );
}

void SDLWindow::drawBorders()
{
	SDL_Rect top;
	SDL_Rect bottom;
	SDL_Rect right;
	SDL_Rect left;
	SDL_SetRenderDrawColor(renderer_, 255, 214, 255, 255);
	
	top.x = 0;
	top.y = 0;
	top.w = width_;
	top.h = 10;
	SDL_RenderFillRect(renderer_, &top);
	bottom.x = 0;
	bottom.y = height_ - 10;
	bottom.w = width_;
	bottom.h = 10;
	SDL_RenderFillRect(renderer_, &bottom);
	right.x = width_ - 10;
	right.y = 0;
	right.w = 10;
	right.h = height_;
	SDL_RenderFillRect(renderer_, &right);
	left.x = 0;
	left.y = 0;
	left.w = 10;
	left.h = height_;
	SDL_RenderFillRect(renderer_, &left);
}

void SDLWindow::closeWindow()
{
	//delete pixels_;
	SDL_DestroyTexture(canvas_);
	SDL_DestroyRenderer(renderer_);
	SDL_DestroyWindow(window_);
	SDL_Quit();
}
