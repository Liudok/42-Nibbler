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
	// const auto response = wgetch(window_);
	// switch(response){
	// 	case 'q': return endGame;
	// 	case 'a': return left;
	// 	case 'd': return right;
	// 	case 'w': return up;
	// 	case 's': return down;
	// 	case 'z': return toNcurses;
	// 	case 'x': return toDummy;
	// 	default: return noResponse;
	// }
// while (SDL_PollEvent(&event_))
// 	{
// 		if (event_.type == SDL_QUIT || (event_.type == SDL_KEYDOWN && event_.key.keysym.sym == SDLK_ESCAPE))
// 			return endGame;
// 		else if (event_.type == SDL_KEYDOWN )
// 		{
// 			if (event_.key.keysym.sym == SDLK_UP)
// 				return up;
// 			else if (event_.key.keysym.sym == SDLK_DOWN)
// 				return down;
// 			else if (event_.key.keysym.sym == SDLK_LEFT)
// 				return left;
// 			else if (event_.key.keysym.sym == SDLK_RIGHT)
// 				return right;
// 		}
// 		else
// 			return noResponse;
// 	}
	return noResponse;
}

void SDLWindow::draw(std::vector<std::vector<size_t>> const& gameState)
{
	gameStateToPixels(gameState);
	SDL_UpdateTexture(canvas_, NULL, pixels_, width_ << 2);
	SDL_RenderClear(renderer_);
	SDL_RenderCopy(renderer_, canvas_, NULL, NULL);
	SDL_RenderPresent(renderer_);
}

void SDLWindow::openWindow(size_t width, size_t height)
{
	width_ = width;
	height_ = height;
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
	pixels_ = (int*)malloc(sizeof(int) * width_ * height_); //Стремный момент
	bzero(pixels_, sizeof(int) * width_ * height_);

}


void SDLWindow::gameStateToPixels(std::vector<std::vector<size_t>> const& gameState)
{
	int color = 18238274;
	const size_t nbRows = gameState.size();
	const size_t nbColumns = gameState[0].size();
	for (size_t i = 0; i < nbRows; ++i){
		for (size_t j = 0; j < nbColumns; ++j){
			if (gameState[i][j] == 0)
				pixels_[i*j +j] = 0;
			else
				pixels_[i*j+j]= color;
		}
	}
}

void SDLWindow::closeWindow()
{
	// delete pixels_;
	// SDL_DestroyTexture(canvas_);
	// SDL_DestroyRenderer(renderer_);
	// SDL_DestroyWindow(window_);
	SDL_Quit();
}
