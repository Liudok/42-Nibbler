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


responseType GlfwWindow::getResponse()
{
    glfwPollEvents();

    if ( glfwGetKey(window_, GLFW_KEY_ESCAPE ) == GLFW_PRESS)
        return endGame;
    else
    {
        if (glfwGetKey(window_, GLFW_KEY_UP )  == GLFW_PRESS || glfwGetKey(window_,GLFW_KEY_W)  == GLFW_PRESS)
            return up;
        else if (glfwGetKey(window_, GLFW_KEY_DOWN) == GLFW_PRESS || glfwGetKey(window_, GLFW_KEY_S ) == GLFW_PRESS)
            return down;
        else if (glfwGetKey(window_, GLFW_KEY_LEFT)  == GLFW_PRESS || glfwGetKey(window_, GLFW_KEY_A) == GLFW_PRESS)
            return left;
        else if (glfwGetKey(window_, GLFW_KEY_RIGHT) == GLFW_PRESS || glfwGetKey(window_, GLFW_KEY_D) == GLFW_PRESS)
            return right;
        else if (glfwGetKey(window_, GLFW_KEY_SPACE ) == GLFW_PRESS)
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

    if( !glfwInit() )
    {
        fprintf( stderr, "Failed to initialize GLFW\n" );
        return ;
    }

    window_ = glfwCreateWindow( (width_ + 2) * zoomFactor_, (height_ + 2) * zoomFactor_, "GLFW Nibbler", NULL, NULL);
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

    glMatrixMode(GL_PROJECTION);

    glLoadIdentity();

    GLfloat zNear = 0.1f;
    GLfloat zFar = 255.0f;
    GLfloat aspect = float(width)/float(height);
    GLfloat fH = tan( 60 / 360.0f * 3.14159f ) * zNear;
    GLfloat fW = fH * aspect;
    glFrustum( -fW, fW, -fH, fH, zNear, zFar );

    GLKMatrix4MakeOrtho(0, (float)(width_ + 1), 0, (float)(height_ + 1), 0, 1);
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
    showText(("Score: " + std::to_string((int)score_)).c_str(), -0.1, -0.1);

    glfwSwapBuffers(window_);
}



void GlfwWindow::gameStateToPixels(field const& gameState)
{
    for (size_t i = 0; i < height_ - 2; ++i)
    {
        for (size_t j = 0; j < width_ - 2; ++j)
        {
            if (gameState[i][j] != 0)
            {
                glColor3f(0.93, 0.96, 0.93);
                if (gameState[i][j] == 1)
                    glColor3f(0.93, 0.96, 0.13);
                else if (gameState[i][j] == 2)
                    glColor3f(0.93, 0.56, 0.93);
                else if (gameState[i][j] == 3)
                    glColor3f(0.13, 0.86, 0.13);
                else if (gameState[i][j] == 4)
                    glColor3f(0.3, 0.1, 0.83);
                float x = -1 + 2 * (((float)j + 0.0) / ((float)width_)) + 0.05;
                float y = 1 - 2 * (((float)i + 0.0) / (float)height_) - 0.1;
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
    showText(("Score: " + std::to_string((int)score_)).c_str(), -0.95, -0.99);
    showText(("Speed: " + std::to_string((int)speed_)).c_str(), 0.65, -0.99);
}

void		GlfwWindow::makeRect(float x, float y, float width, float height)
{
    glLoadIdentity();
    glBegin(GL_QUADS);
    glVertex3f((GLfloat)x, (GLfloat)y, 0.0f);              // Top Left
    glVertex3f((GLfloat)x + width, (GLfloat)y, 0.0f);      // Top Right
    glVertex3f((GLfloat)x + width, (GLfloat)y + height, 0.0f);  // Bottom Right
    glVertex3f((GLfloat)x, (GLfloat)y + height, 0.0f);          // Bottom Left
    glEnd();
}

void  GlfwWindow::showText(const char *text, float x, float y)
{
    glDisable(GL_TEXTURE_2D);
    glDisable(GL_TEXTURE);

    glLoadIdentity();
    glRasterPos2f(x, y);

    size_t len = strlen(text);
    size_t i = 0;

    while (i < len)
    {
        glutBitmapCharacter(GLUT_BITMAP_HELVETICA_18 , text[i]);
        i++;
    }
    glEnable(GL_TEXTURE_2D);
}

GlfwWindow::~GlfwWindow()
{

}