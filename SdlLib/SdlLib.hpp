#pragma once
#include "../IWindow/IWindow.hpp"
#include "/Library/Frameworks/SDL2.framework/Versions/Current/Headers/SDL.h"

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
		size_t			width_ = 300;
		size_t			height_ = 500;
		SDL_Window		*window_ = nullptr;
		SDL_Renderer	*renderer_;
		SDL_Texture		*canvas_;
		// SDL_Event		event_;
		int				*pixels_;
};
