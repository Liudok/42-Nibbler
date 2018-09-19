#include "GlfwLib.hpp"
#include <unistd.h>
#include <string>
#include <functional>
#include <iostream>
#include <cmath>
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
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glLoadIdentity();



//    float ratio;
//    int width, height;
//    glfwGetFramebufferSize(window_, &width, &height);
//    ratio = width / (float) height;
//    glViewport(0, 0, width, height);
//    glClear(GL_COLOR_BUFFER_BIT);
//    glMatrixMode(GL_PROJECTION);
//    glLoadIdentity();
//    glOrtho(-ratio, ratio, -1.f, 1.f, 1.f, -1.f);
//    glMatrixMode(GL_MODELVIEW);
//    glLoadIdentity();
//    glRotatef((float) glfwGetTime() * 50.f, 0.f, 0.f, 1.f);
//    glBegin(GL_TRIANGLES);
//    glColor3f(1.f, 0.f, 0.f);
//    glVertex3f(-0.6f, -0.4f, 0.f);
//    glColor3f(0.f, 1.f, 0.f);
//    glVertex3f(0.6f, -0.4f, 0.f);
//    glColor3f(0.f, 0.f, 1.f);
//    glVertex3f(0.f, 0.6f, 0.f);
//    glEnd();



         // Clear The Screen And The Depth Buffer
                          // Reset The Current Modelview Matrix


//    glTranslatef(3.0f,0.0f,0.0f);                   // Move Right 3 Units
//    glColor3f(1.f,0.f,1.0f);              // Set The Color To Blue One Time Only
//    glBegin(GL_QUADS);                      // Draw A Quad
//    glVertex3f(-1.0f, 1.0f, 0.0f);              // Top Left
//    glVertex3f( 1.0f, 1.0f, 0.0f);              // Top Right
//    glVertex3f( 1.0f,-1.0f, 0.0f);              // Bottom Right
//    glVertex3f(-1.0f,-1.0f, 0.0f);              // Bottom Left
//    glEnd();                            // Done Drawing The Quad
//    glLoadIdentity();                           // Reset The Current Modelview Matrix

    gameStateToPixels(gameState);
    glFlush();
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

    window_ = glfwCreateWindow( width_ * zoomFactor_, height_ * zoomFactor_, "GLFW Nibbler", NULL, NULL);
    if( !window_)
    {
        fprintf( stderr, "Failed to open GLFW window. If you have an Intel GPU, they are not 3.3 compatible. Try the 2.1 version of the tutorials.\n" );
        glfwTerminate();
        return ;
    }
    glfwMakeContextCurrent(window_);
    glfwSwapInterval(1);

    glfwSetInputMode(window_, GLFW_STICKY_KEYS, GL_TRUE);

    glClearColor(0.1f, 0.2f, 0.4f, 0.0f);
//    GLint windowWidth, windowHeight;
//    glfwGetWindowSize(window_, &windowWidth, &windowHeight);
//    glViewport(0, 0, windowWidth, windowHeight);
    glMatrixMode(GL_PROJECTION);

    glLoadIdentity();

//    GLfloat zNear = 0.1f;
//    GLfloat zFar = 255.0f;
//    GLfloat aspect = float(width)/float(height);
//    GLfloat fH = tan( float(60 / 360.0f * 3.14159f) ) * zNear;
//    GLfloat fW = fH * aspect;
//    glFrustum( -fW, fW, -fH, fH, zNear, zFar );

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
    glDepthFunc(GL_LEQUAL);                         // The Type Of Depth Test To Do
    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);

    glDisable(GL_CULL_FACE);
    glCullFace(GL_BACK);
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



void GlfwWindow::gameStateToPixels(field const& gameState)
{
//    glPointSize(10);
//    glLineWidth(2.5);
//    glColor3f(1.0, 0.0, 0.0);
//    glBegin(GL_LINES);
//    glVertex3f(-1.0, 1.0,0.0);
//    glVertex3f(-1.0, 0.0,0.0);
//    glEnd();
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
//    glVertex2f(0.0, 0.0);
//    glVertex2f(0.0, -0.5);
//    glVertex2f(-0.5, -0.5);
//    glVertex2f(-0.5, 0.0);
//
//    glEnd();

  //glBegin(GL_TRIANGLES);                      // Drawing Using Triangles
    glBegin(GL_QUADS);
//    glColor3f(1.0f,0.0f,0.0f);          // Set The Color To Red
//    glVertex3f( 0.0f, 1.0f, 0.0f);          // Move Up One Unit From Center (Top Point)

    glColor3f(0.0f,1.0f,0.0f);          // Set The Color To Green
    glVertex3f(-1.0f,-1.0f, 0.0f);          // Left And Down One Unit (Bottom Left)

    glColor3f(0.0f,0.0f,1.0f);          // Set The Color To Blue
    glVertex3f( 1.0f,-1.0f, 0.0f);          // Right And Down One Unit (Bottom Right)

    glColor3f(1.0f,1.0f,1.0f);          // Set The Color To Blue
    glVertex3f( 1.0f,1.0f, 0.0f);

    glColor3f(1.0f,0.0f,0.0f);          // Set The Color To Red
    glVertex3f( -1.0f, 1.0f, 0.0f);

    glEnd();                            // Finished Drawing The Triangle

    for (size_t i = 0; i < height_; ++i)
    {
        for (size_t j = 0; j < width_; ++j)
        {
            if (gameState[i][j] != 0)
            {
                glColor3f(0.93, 0.96, 0.93);
                if (gameState[i][j] == 1)
                    glColor3f(0.93, 0.56, 0.93);
                else if (gameState[i][j] == 2)
                {
                    glColor3f(0.93, 0.96, 0.13);
                    std::cout <<"x =  " << (0.025 + (float)j / (float)width_) << " y = " <<  (0.025 + (float)i / (float)height_ ) << std::endl;

                }
                else if (gameState[i][j] == 3)
                    glColor3f(0.13, 0.86, 0.13);
                else if (gameState[i][j] == 4)
                    glColor3f(0.3, 0.1, 0.83);
                float x = 0 - 0.025 + (float)j / (float)width_;
                float y = 0 - 0.025 + (float)i / (float)height_;
                makeRect(x, y, 0.1, 0.1);
            }
        }
    }
    drawBorders();
}

void GlfwWindow::drawBorders()
{
    glColor3f(0.53, 0.56, 0.83);
    makeRect(-1, -1, width_ * 0.1, 0.1);
    makeRect(-1, 0.9, width_ * 0.1, 0.1);
    makeRect(-1, 0.1, 0.1, 1);
    makeRect(0.9, 0.1, 0.1, -1);
    makeRect(0.9, 0.1, 0.1, 1);
    makeRect(-1, 0.1, 0.1, -1);
}

void		GlfwWindow::makeRect(float x, float y, float width, float height)
{
    glBegin(GL_QUADS);
    glVertex3f((GLfloat)x, (GLfloat)y, 0.0f);              // Top Left
    glVertex3f((GLfloat)x + width, (GLfloat)y, 0.0f);              // Top Right
    glVertex3f((GLfloat)x + width, (GLfloat)y + height, 0.0f);              // Bottom Right
    glVertex3f((GLfloat)x, (GLfloat)y + height, 0.0f);              // Bottom Left
    glEnd();
    glLoadIdentity();
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




GlfwWindow::~GlfwWindow()
{

}