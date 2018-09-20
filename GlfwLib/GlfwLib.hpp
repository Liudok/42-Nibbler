#pragma once

#include <Nibbler.hpp>
#include <GLFW/glfw3.h>
#include <GLUT/glut.h>
#include <array>
#include <functional>

extern "C"
{
    IWindow* create();
}

class GlfwWindow : public IWindow
{
    public:

        GlfwWindow() = default;
        GlfwWindow(GlfwWindow const&) = delete;
        GlfwWindow& operator=(GlfwWindow const&) = delete;
        ~GlfwWindow() = default;

        ResponseType    getResponse() override;
        void            draw(GameField const&, size_t score, double speed, GameMode) override;
        void            openWindow(size_t width, size_t height) override;
        void            closeWindow() override;
        void            showGameOver() override;

    private:

        void            gameStateToPixels(GameField const&);
        void            makeRect(float x, float y, float width, float height);
        void            showText(const char *text, float x, float y);
        void            drawBorders();

        const size_t    zoomFactor_ = defaultZoomFaftor * 2.0 / 3;
        size_t          width_ = defaultWidth;
        size_t          height_ = defaultHeight;
        GLFWwindow*     window_ = nullptr;
        size_t          score_ = defaultScore;
        double          speed_ = defaultSpeed;
        GameMode        mode_ = defaultGameMode;

        inline static double rc()
        { return static_cast<double>(rand()) / (RAND_MAX); }

        inline void setColor(double r, double g, double b)
        { (mode_ == rasta) ?
            glColor3f(rc(), rc(), rc()) :
                glColor3f(r, g, b); }

        using ColorFunctionsArray =
            std::array<std::function<void()>, nbGameFieldCellTypes>;
        ColorFunctionsArray setColor_ = initColorFunctionsArray();

        ColorFunctionsArray initColorFunctionsArray();

        std::array<size_t, nbResponses> initResponses();
        std::array<size_t, nbResponses> responses_ = initResponses();
};
