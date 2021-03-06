#include "GlfwLib.hpp"
#include <string>
#include <cmath>

extern "C"
{
    IWindow* create()
    {
        return new GlfwWindow;
    }
}

ResponseType GlfwWindow::getResponse()
{
    glfwPollEvents();
    for (size_t i = 0; i < nbResponses; ++i)
        if (glfwGetKey(window_, responses_[i]) == GLFW_PRESS)
            return static_cast<ResponseType>(i);
    return noResponse;
}

void GlfwWindow::draw(GameField const& gameState, size_t score, double speed, GameMode mode)
{
    score_ = score;
    speed_ = speed;
    mode_ = mode;

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();

    glBegin(GL_QUADS);

    glColor3f(0.0f,1.0f,0.0f);
    glVertex3f(-1.0f,-1.0f, 0.0f);

    glColor3f(0.0f,0.0f,1.0f);
    glVertex3f( 1.0f,-1.0f, 0.0f);

    glColor3f(1.0f,1.0f,1.0f);
    glVertex3f( 1.0f,1.0f, 0.0f);

    glColor3f(1.0f,0.0f,0.0f);
    glVertex3f( -1.0f, 1.0f, 0.0f);

    glEnd();

    gameStateToPixels(gameState);

    glFlush();
    glfwSwapBuffers(window_);
}

void GlfwWindow::openWindow(size_t width, size_t height)
{
    width_ = width + 2;
    height_ = height + 2;

    if(!glfwInit())
        throw std::runtime_error("Failed to initialize GLFW");

    window_ = glfwCreateWindow((width_ + 2) * zoomFactor_, 
        (height_ + 2) * zoomFactor_, "GLFW Nibbler", NULL, NULL);
    
    if(!window_){
        glfwTerminate();
        throw std::runtime_error("Failed to initialize GLFW window");
    }

    glfwMakeContextCurrent(window_);
    glfwSwapInterval(1);

    glfwSetInputMode(window_, GLFW_STICKY_KEYS, GL_TRUE);

    glClearColor(0.1f, 0.2f, 0.4f, 0.0f);
}

void GlfwWindow::closeWindow()
{
    glfwDestroyWindow(window_);
    glfwTerminate();
}

void GlfwWindow::showGameOver()
{
    glClearColor(0.9f, 0.7f, 0.2f, 0.0f);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glColor3f(0.43, 0.35, 0.63);
    showText("Game over", -0.1, 0);

    glColor3f(0.99, 0.35, 0.63);
    showText(("Score: " + std::to_string(score_)).c_str(), -0.1, -0.1);

    glfwSwapBuffers(window_);
}

void GlfwWindow::gameStateToPixels(GameField const& gameState)
{
    for (size_t i = 0; i < height_ - 2; ++i)
    {
        for (size_t j = 0; j < width_ - 2; ++j)
        {
            if (gameState[i][j] != 0)
            {
                setColor_[gameState[i][j]]();
                const auto x = -1 + 2.0 * j / width_ + 0.05;
                const auto y = 1 - 2.0 * i / height_ - 0.1;
                makeRect(x, y, 0.05, 0.05);
            }
        }
    }
    drawBorders();
}

void GlfwWindow::drawBorders()
{
    glColor3f(0.43, 0.35, 0.63);
    makeRect(-1, -1, 2, 0.05); // bottom
    makeRect(-1, 0.95, 2, 0.05); //top
    makeRect(-1, 0.05, 0.05, 0.95); //left
    makeRect(-1, 0.05, 0.05, -1); //left
    makeRect(0.95,0.05, 0.05, 0.95);
    makeRect(0.95,0.05, 0.05, -1);

    glColor3f(0.99, 0.35, 0.63);
    showText(("Score : " + std::to_string(score_)).c_str(), -0.95, -0.99);
    showText(("Speed : " + std::to_string(static_cast<size_t>(speed_ * 100))).c_str(), 0.65, -0.99);
}

void		GlfwWindow::makeRect(float x, float y, float width, float height)
{
    glLoadIdentity();
    glBegin(GL_QUADS);
    glVertex3f((GLfloat)x, (GLfloat)y, 0.0f); // Top Left
    glVertex3f((GLfloat)x + width, (GLfloat)y, 0.0f); // Top Right
    glVertex3f((GLfloat)x + width, (GLfloat)y + height, 0.0f); // Bottom Right
    glVertex3f((GLfloat)x, (GLfloat)y + height, 0.0f); // Bottom Left
    glEnd();
}

void  GlfwWindow::showText(const char *text, float x, float y)
{
    glLoadIdentity();
    glRasterPos2f(x, y);

    const auto len = std::strlen(text);
    for (size_t i = 0; i < len; ++i)
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18 , text[i]);
}

auto GlfwWindow::initColorFunctionsArray()
    -> ColorFunctionsArray
{
    return{{
        [](){ glColor3f(0.3, 0.51, 0.07); },
        [this](){ setColor(0.49, 1, 0.95); },
        [this](){ setColor(0.25, 0.97, 0.87); },
        [this](){ setColor(1, 0.45, 0.83); },
        [](){ glColor3f(rc(), rc(), rc()); },
        [](){ glColor3f(1, 0, 0); },
        [this](){ setColor(0.04, 0.44, 0.59); }
    }};
}

std::array<size_t, nbResponses>GlfwWindow::initResponses()
{
    std::array<size_t, nbResponses> responses =
        {{
             0,
             GLFW_KEY_LEFT,
             GLFW_KEY_RIGHT,
             GLFW_KEY_UP,
             GLFW_KEY_DOWN,
             5,
             GLFW_KEY_1,
             GLFW_KEY_2,
             GLFW_KEY_SPACE,
             GLFW_KEY_4,
             GLFW_KEY_ESCAPE
         }};
    return responses;
}
