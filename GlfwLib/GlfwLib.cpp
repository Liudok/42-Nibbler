#include "GlfwLib.hpp"
#include <unistd.h>
#include <string>
#include <functional>

extern "C"
{
    IWindow* create()
    {
        return new GlfwWindow;
    }
}

#define SQUARE_SIZE 10
//#include FT_FREETYPE_H

responseType GlfwWindow::getResponse()
{
    glfwPollEvents();

    if ( glfwGetKey(window_, GLFW_KEY_ESCAPE ) == GLFW_PRESS)
        return endGame;
    else
    {
        if (glfwGetKey(window_, GLFW_KEY_UP )  == GLFW_PRESS || glfwGetKey(window_,GLFW_KEY_UP)  == GLFW_PRESS)
            return up;
        else if (glfwGetKey(window_, GLFW_KEY_DOWN) == GLFW_PRESS || glfwGetKey(window_, GLFW_KEY_DOWN ) == GLFW_PRESS)
            return down;
        else if (glfwGetKey(window_, GLFW_KEY_LEFT)  == GLFW_PRESS || glfwGetKey(window_, GLFW_KEY_A) == GLFW_PRESS)
            return left;
        else if (glfwGetKey(window_, GLFW_KEY_RIGHT) == GLFW_PRESS || glfwGetKey(window_, GLFW_KEY_RIGHT) == GLFW_PRESS)
            return right;
        else if (glfwGetKey(window_, GLFW_KEY_P ) == GLFW_PRESS)
            return pauseContinue;
        else if (glfwGetKey(window_, GLFW_KEY_1 ) == GLFW_PRESS)
            return toSDL;
        else if (glfwGetKey(window_, GLFW_KEY_2 ) == GLFW_PRESS)
            return toSFML;
    }
    return noResponse;
}

void GlfwWindow::draw(field const& gameState, size_t score, size_t speed)
{
    score_ = score;
    speed_ = speed;
    gameStateToPixels(gameState);
    glfwSwapBuffers(window_);
}

void GlfwWindow::openWindow(size_t width, size_t height)
{
    width_ = width;
    height_ = height;

    if( !glfwInit() )
    {
        fprintf( stderr, "Failed to initialize GLFW\n" );
        return ;
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE); // To make MacOS happy; should not be needed
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

    window_ = glfwCreateWindow( width_ * zoomFactor_, height_ * zoomFactor_, "GLFW Nibbler", NULL, NULL);
    if( !window_)
    {
        fprintf( stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n" );
        glfwTerminate();
        return ;
    }
    glfwMakeContextCurrent(window_);

    glfwSetInputMode(window_, GLFW_STICKY_KEYS, GL_TRUE);

    glClearColor(0.1f, 0.2f, 0.4f, 0.0f);
    //Init FreeType
//    FT_Library ft;
//    if (FT_Init_FreeType(&ft)) {
//        fprintf(stderr, "FATAL: Could not init FreeType");
//        return 1;
//    }
//    //Init Arial FreeType Face
//    FT_Face arial;
//    if (FT_New_Face(ft, "Arial", 0, &arial))
//    {
//        fprintf(stderr, "FATAL: Could not init font \"Arial\"");
//        return 1;
//    }
}

void GlfwWindow::closeWindow()
{
    glfwTerminate();
}

void GlfwWindow::showGameOver()
{
//    SDL_Color color = {199, 50, 176, 0};
//    showText("Game over", (width_ / 2) * 4 - 5, (height_ / 2) * 4, color);
//    color = {59, 150, 116, 0};
//    std::string score("Score: " + std::to_string((int)score_));
//    const char *text = score.c_str();
//    showText(text, (width_ / 2) * 4, (height_ / 2) * 4 + 20, color);
//    SDL_RenderPresent(renderer_);
}




GlfwWindow::~GlfwWindow()
{
    
}

void GlfwWindow::gameStateToPixels(field const& gameState)
{
    glClear(GL_COLOR_BUFFER_BIT);


    int width, height;

    glfwGetFramebufferSize ( window_, &width, &height );

    float   ratio = width / (float) height;

    glViewport(0, 0, width, height);
    glClear(GL_COLOR_BUFFER_BIT);
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(-ratio, ratio, -1.f, 1.f, 1.f, -1.f);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    glRotatef((float) glfwGetTime() * 50.f, 0.f, 0.f, 1.f);
    glBegin(GL_TRIANGLES);
    glColor3f(1.f, 0.f, 0.f);
    glVertex3f(-0.6f, -0.4f, 0.f);
    glColor3f(0.f, 1.f, 0.f);
    glVertex3f(0.6f, -0.4f, 0.f);
    glColor3f(0.f, 0.f, 1.f);
    glVertex3f(0.f, 0.6f, 0.f);
    glEnd();

    //    std::function<void()> setColor[nbGameFieldCellTypes] = {
//        [this](){ return SDL_SetRenderDrawColor(renderer_, 79, 132, 196, 255); },
//        [this](){ return SDL_SetRenderDrawColor(renderer_, 127, 255, 212, 255); },
//        [this](){ return SDL_SetRenderDrawColor(renderer_, 64, 224, 208, 255); },
//        [this](){ return SDL_SetRenderDrawColor(renderer_, 255, 105, 180, 255); },
//        [this](){ return SDL_SetRenderDrawColor(renderer_, 248, 14, 50, 255); },
//        [this](){ return SDL_SetRenderDrawColor(renderer_, 11, 111, 144, 244); }
//    };
    for (size_t i = 0; i < height_; ++i)
    {
        for (size_t j = 0; j < width_; ++j)
        {
//            SDL_Rect rectangle;
//            rectangle.x = (j + 1) * zoomFactor_;
//            rectangle.y = (i + 1) * zoomFactor_;
//            rectangle.w = zoomFactor_;
//            rectangle.h = zoomFactor_;
//            setColor[gameState[i][j]]();
            gameState[i][j];
//            SDL_RenderFillRect(renderer_, &rectangle);
        }
    }
//    drawBorders();

}

void GlfwWindow::drawBorders()
{
//    SDL_Rect top;
//    SDL_Rect bottom;
//    SDL_Rect right;
//    SDL_Rect left;
//    SDL_SetRenderDrawColor(renderer_, 95, 158, 160, 255);
//
//    top.x = 0;
//    top.y = 0;
//    top.w = (width_ + 1) * zoomFactor_;
//    top.h = zoomFactor_;
//    SDL_RenderFillRect(renderer_, &top);
//
//    bottom.x = 0;
//    bottom.y = (height_ + 1) * zoomFactor_;
//    bottom.w = (width_ + 2) * zoomFactor_;
//    bottom.h = zoomFactor_;
//    SDL_RenderFillRect(renderer_, &bottom);
//
//    right.x = (width_ + 1) * zoomFactor_;
//    right.y = 0;
//    right.w = zoomFactor_;
//    right.h = (height_ + 1) * zoomFactor_;
//    SDL_RenderFillRect(renderer_, &right);
//
//    left.x = 0;
//    left.y = 0;
//    left.w = zoomFactor_;
//    left.h = (height_ + 1) * zoomFactor_;
//    SDL_RenderFillRect(renderer_, &left);
//
//    const auto score = "score: " + std::to_string(score_);
//    showText(score.c_str(), 0, 0, {199, 50, 176, 0});
//    const auto speed = "speed: " + std::to_string(speed_);
//    showText(speed.c_str(), (width_ / 2.4) * zoomFactor_, 0, {199, 50, 176, 0});
}

//void		GlfwWindow::makeRect(int x, int y, int width, int height)
//{
//    glBegin(GL_QUADS);
//    glVertex2f((GLfloat)x, (GLfloat)y);
//    glVertex2f((GLfloat)x + width, (GLfloat)y);
//    glVertex2f((GLfloat)x + width, (GLfloat)y + height);
//    glVertex2f((GLfloat)x, (GLfloat)y + height);
//    glEnd();
//}


void GlfwWindow::makeRect(size_t x, size_t y, size_t h, size_t w)
{
    x = x * SQUARE_SIZE * 2;
    y = y * SQUARE_SIZE * 2;
    glRecti(x, y, w, h);
}

void  GlfwWindow::showText(const char *text, size_t x, size_t y)
{
    int     len;
    int     i;

    glRasterPos2f(x, y);
    len = strlen(text);
    i = 0;
    while (i < len)
    {
      //  glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, text[i]);
        i++;
    }
}
