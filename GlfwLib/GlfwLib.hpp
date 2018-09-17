#pragma once

#include <IWindow.hpp>
#include <GLFW/glfw3.h>

extern "C"
{
    IWindow* create();
}

class GlfwWindow : public IWindow
{
	public:

        responseType    getResponse() override;
        void            draw(field const&, size_t score, size_t speed) override;
        void            openWindow(size_t width, size_t height) override;
        void            closeWindow() override;
        void            showGameOver() override;
        ~GlfwWindow();

    private:

        void            gameStateToPixels(std::vector<std::vector<size_t>> const& gameState);
        void            drawBorders();
       // Glfw_Rect        makeRect(size_t x, size_t y, size_t h, size_t w);
        void            showText(const char *text, size_t x, size_t y, Glfw_Color color);

        const size_t    zoomFactor_ = 14;
        size_t          width_ = 0;
        size_t          height_ = 0;
        GLFWwindow*     window_;
//        Glfw_Window      *window_ = nullptr;
//        Glfw_Renderer    *renderer_ = nullptr;
        size_t          score_ = 0;
        size_t          speed_ = 0;
};
