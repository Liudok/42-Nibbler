#pragma once
#include "../IWindow/IWindow.hpp"
#include "SFML/include/SFML/Graphics.hpp"
#include "SFML/include/SFML/Window.hpp"
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
		size_t			width_;
		size_t			height_;
		sf::RenderWindow	*window_;
		bool paused_ = false;
};
