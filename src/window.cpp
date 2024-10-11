#include "window.h"

//
//
//  Creates a SDL_Window and assigns the param values to private class members
//
//
GLWindow::GLWindow(const char *title, unsigned int h_pos, unsigned int v_pos, unsigned int width, unsigned int height, Uint8 window_args)
{
    this->window = SDL_CreateWindow(title, h_pos, v_pos, width, height, window_args);
    this->title = title;
    this->width = width;
    this->height = height;
    this->max_fps = 60;
}

//
//
//  The destrcutor for the GLWindow class
//
//
GLWindow::~GLWindow()
{
    if (this->window != nullptr)
        SDL_DestroyWindow(this->window);
}

//
//
//  Sets the window title to the param value if the param is valid
//
//
bool GLWindow::SetWindowTitle(const char *title)
{
    if (title == nullptr || title == 0)
        return false;
    this->title = title;
    SDL_SetWindowTitle(window, title);
    return true;
}

//
//
//  Returns the window title
//
//
const char *GLWindow::GetWindowTtitle()
{
    return this->title;
}

//
//
//  Sets the window size if both values aren't 0, if one of the values are zero the other gets passed to the window
//
//
bool GLWindow::SetWindowSize(unsigned int width, unsigned int height)
{
    if (width == 0 && height == 0)
        return false;
    if (width != 0)
        this->width = width;
    if (height != 0)
        this->height = height;

    SDL_SetWindowSize(window, this->width, this->height);
    return true;
}

//
//
//  Simply returns the window width
//
//
unsigned int GLWindow::GetWindowWidth()
{
    return this->width;
}

//
//
//  Returns the window height
//
//
unsigned int GLWindow::GetWindowHeight()
{
    return this->height;
}

//
//
//  Sets the max fps for the window
//
//
bool GLWindow::SetMaxFPS(unsigned int fps)
{
    if(fps = 0)
    {
        std::cout << "Invalid max fps: " << fps << std::endl;
        return false;
    }

    this->max_fps = fps;

    return true;
}

//
//
//  Gets the max fps for the window
//
//
unsigned int GLWindow::GetMaxFPS()
{
    return this->max_fps;
}

//
//
//  Tests to see if the window should close
//
//
bool GLWindow::ShouldClose()
{
    return !this->running;
}

//
//
//  Sets weather the window should close
//
//
void GLWindow::SetShouldClose(bool shouldClose)
{
    this->running = !shouldClose;
}

//
//
//  Tests to see if the window is not a nullptr or null
//
//
bool GLWindow::IsWindowValid(GLWindow *window)
{
    return window != nullptr && window != NULL;
}

//
//
// Creates a Opengl context based off of the window
//
//
SDL_GLContext GLWindow::CreateContext()
{
    if (!IsWindowValid(this))
    {
        std::cout << "Window not valid for context creation\n";
        return NULL;
    }

    return SDL_GL_CreateContext(this->window);
}

//
//
//  Sets the context as current
//
//
bool GLWindow::MakeContextCurrent(SDL_GLContext ctx)
{
    if (!ctx)
        return false;

    SDL_GL_MakeCurrent(this->window, ctx);

    return true;
}

//
//
//  Clears the window and checks for gl errors
//
//
bool GLWindow::ClearWindow()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    GLenum error = glGetError();
    if(error != GL_NO_ERROR)
    {
        std::cout << "Error clearing the screen: " << error << std::endl;
        return false;
    }
    return true;
}

//
//
//  Swaps the window buffers and checks for gl errors
//
//
bool GLWindow::SwapWindow()
{
    SDL_GL_SwapWindow(this->window);

    GLenum error = glGetError();
    if(error != GL_NO_ERROR)
    {
        std::cout << "Error swapping the window: " << error << std::endl;
        return false;
    }

    return true;
}

//
//
// Returns the wait time calculated with the current delta time
//
//
float GLWindow::GetWaitTime(float dt)
{
    float frameTime = (float)(1 / this->max_fps) * 1000.0f;
    return dt - frameTime;
}

//
//
//  Returns the raw SDL_Window
//
//
SDL_Window *GLWindow::GetWindow()
{
    return this->window;
}
