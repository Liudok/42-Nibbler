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

		responseType	 getResponse() override;
		void			 draw(std::vector<std::vector<size_t>> const&) override;
		void			 openWindow(size_t width, size_t height) override;
		void			 closeWindow() override;
		void             setScore(size_t score) override;
		void             setSpeed(size_t speed) override;
		void             showGameOver() override;
		~SFMLWindow();

	private:

		void			  gameStateToPixels(std::vector<std::vector<size_t>> const& gameState);
		void 			  drawBorders();
		bool			  isPaused();
		size_t			  width_ = 0;
		size_t			  height_ = 0;
		size_t			  score_ = 0;//no need to save
		size_t			  speed_ = 0;//no need to save
		sf::RenderWindow  *window_ = nullptr;
		bool              paused_ = false;
};
