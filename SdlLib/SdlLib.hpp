#pragma once
#include "../IWindow/IWindow.hpp"
#include "SDL.h"
//#include "SDL_image.h"
extern "C"
{
	IWindow* create();
}

class SDLWindow : public IWindow
{
	public:

		responseType	getResponse() override;
		void			draw(std::vector<std::vector<size_t>> const&) override;
		void			openWindow(size_t width, size_t height) override;
		void			closeWindow() override;

	private:

		void			gameStateToPixels(std::vector<std::vector<size_t>> const& gameState);
		void 			drawBorders();
		bool			isPaused();
		size_t			width_;
		size_t			height_;
		SDL_Window		*window_ = nullptr;
		SDL_Renderer	*renderer_;
		SDL_Event  		event_;
		bool paused_ = false;
};
