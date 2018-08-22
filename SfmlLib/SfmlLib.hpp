#pragma once

#include <IWindow.hpp>
#include <SFML/Graphics.hpp>
#include <SFML/Window.hpp>

extern "C"
{
	IWindow* create();
}

class SFMLWindow : public IWindow
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
		size_t			width_ = 0;
		size_t			height_ = 0;
		sf::RenderWindow	*window_ = nullptr;
		bool paused_ = false;
};
