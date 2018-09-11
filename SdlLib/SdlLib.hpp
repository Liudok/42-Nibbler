#pragma once
#include <IWindow.hpp>
#include <SDL.h>

// #include <SDL2/SDL_ttf.h>
#include <SDL_ttf.h>

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
		void            setScore(size_t score) override;
		void            setSpeed(size_t speed) override;
		void            showGameOver() override;
		void			closeWindow() override;

	private:

		void			gameStateToPixels(std::vector<std::vector<size_t>> const& gameState);
		void 			drawBorders();
		bool			isPaused();
		SDL_Rect        makeRect(size_t x, size_t y, size_t h, size_t w);
		size_t			width_;
		size_t			height_;
		SDL_Window		*window_ = nullptr;
		SDL_Renderer	*renderer_;
		SDL_Event  		event_;
		size_t			score_ = 0;
		size_t			speed_ = 0;
		bool paused_ = false;
};
