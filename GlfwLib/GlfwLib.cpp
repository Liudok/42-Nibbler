#include "GlfwLib.hpp"
#include <unistd.h>
#include <string>
#include <functional>
#include <iostream>
extern "C"
{
    IWindow* create()
    {
        return new GlfwWindow;
    }
}

#define SQUARE_SIZE 10

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
//
//    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
//
//    glMatrixMode(GL_MODELVIEW);
//    glLoadIdentity();
//
//    glPointSize(10);
//    glLineWidth(2.5);
//    glColor3f(1.0, 0.0, -10.0);
//    glBegin(GL_LINES);
//    glVertex3f(10.0,10.0,-10.0);
//    glVertex3f(50.0,50.0,-10.0);
//    glEnd();

    float ratio;
    int width, height;
    glfwGetFramebufferSize(window_, &width, &height);
    ratio = width / (float) height;
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

    gameStateToPixels(gameState);
    //glFlush();
    std::cout<<"leaving loop \n";
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
    GLint windowWidth, windowHeight;
    glfwGetWindowSize(window_, &windowWidth, &windowHeight);
    glViewport(0, 0, windowWidth, windowHeight);
    glMatrixMode(GL_PROJECTION);

    glLoadIdentity();

   // gluPerspective( 65.0f, (GLfloat)width/(GLfloat)height, 1.0f, 100.0f );

    glOrtho(0.0, width_, height_, 0, 0, 1.0);

    glEnable(GL_SMOOTH);		// Enable (gouraud) shading

    glDisable(GL_DEPTH_TEST); 	// Disable depth testing
    glEnable(GL_COLOR_MATERIAL);

    glEnable(GL_BLEND);		// Enable blending (used for alpha) and blending function to use
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glLineWidth(5.0f);		// Set a 'chunky' line width

    glEnable(GL_LINE_SMOOTH);	// Enable anti-aliasing on lines

    glPointSize(5.0f);		// Set a 'chunky' point size

    glEnable(GL_POINT_SMOOTH);	// Enable anti-aliasing on points
//    glDepthFunc(GL_LEQUAL);
//    glDisable(GL_CULL_FACE);
//    glCullFace(GL_BACK);
}

void GlfwWindow::closeWindow()
{
    glfwDestroyWindow(window_);
    glfwTerminate();
}

void GlfwWindow::showGameOver()
{
    glClearColor(0.9f, 0.2f, 0.1f, 0.0f);

    glfwSwapBuffers(window_);
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
//    glClear(GL_COLOR_BUFFER_BIT);
//
//    glColor3f(0.25, 0.87, 0.81);
//    glBegin(GL_TRIANGLES);
//
//    glVertex2f(0.0, 0.0);
//    glVertex2f(10.0, 10.0);
//    glVertex2f(10.0, 0.0);
//
//    glEnd();
//
//    glColor3f(0.13, 0.56, 0.13);
//    glBegin(GL_QUADS);
//
//    glVertex2f(20.0, 20.5);
//    glVertex2f(20.0, 40.0);
//    glVertex2f(20.5, 40.0);
//    glVertex2f(20.5, 20.5);
//
//    glEnd();

    for (size_t i = 0; i < height_; ++i)
    {
        for (size_t j = 0; j < width_; ++j)
        {


            if (gameState[i][j] == 0)
            {
                glColor3f(0.93, 0.56, 0.13);
            }
            else
            {
                if (gameState[i][j] == 1)
                    glColor3f(0.93, 0.56, 0.93);
                else if (gameState[i][j] == 2)
                    glColor3f(0.93, 0.96, 0.13);
                else if (gameState[i][j] == 3)
                    glColor3f(0.03, 0.56, 0.13);
                else if (gameState[i][j] == 4)
                    glColor3f(0.3, 0.1, 0.83);
            }
            makeRect(j * 10 + 10, i * 10 + 10, 10, 10);
        }
    }
    drawBorders();
}

void GlfwWindow::drawBorders()
{
    glColor3f(0.13, 0.56, 0.13);
    glBegin(GL_QUADS);

    glVertex2f(0.0, 30.0);
    glVertex2f(0.0, 0.0);
    glVertex2f(40.0, 0.0);
    glVertex2f(40.0, 30.0);

    glEnd();

    glBegin(GL_TRIANGLES);

    glVertex3f(1.0f, 0.5f, 4.0f);    // lower left vertex
    glVertex3f( 2.0f, 0.5f, 4.0f);    // lower right vertex
    glVertex3f( 0.0f,  1.5f, 4.0f);    // upper vertex

    glEnd();
   // glRecti(20, 20, 20, 20);
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

void		GlfwWindow::makeRect(int x, int y, int width, int height)
{
    glBegin(GL_QUADS);
    glVertex2f((GLfloat)x, (GLfloat)y);
    glVertex2f((GLfloat)x + width, (GLfloat)y);
    glVertex2f((GLfloat)x + width, (GLfloat)y + height);
    glVertex2f((GLfloat)x, (GLfloat)y + height);
    glEnd();
}


//void GlfwWindow::makeRect(int x, int y, int width, int height)
//{
//    x = x * SQUARE_SIZE * 2;
//    y = y * SQUARE_SIZE * 2;
//    glRecti(x, y, width, height);
//}

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
