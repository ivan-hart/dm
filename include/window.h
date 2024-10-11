#ifndef WINDOW_H
#define WINDOW_H

#include <glad/glad.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>

#include <iostream>

#define CENTERED SDL_WINDOWPOS_CENTERED
#define SHOWN SDL_WINDOW_SHOWN
#define OPENGL SDL_WINDOW_OPENGL

class GLWindow
{
public:
    GLWindow(const char *title, unsigned int h_pos, unsigned int v_pos, unsigned int width, unsigned int height, Uint8 window_args);
    ~GLWindow();

    bool SetWindowTitle(const char *title);
    const char *GetWindowTtitle();

    bool SetWindowSize(unsigned int width, unsigned int height);
    unsigned int GetWindowWidth();
    unsigned int GetWindowHeight();

    bool SetMaxFPS(unsigned int fps);
    unsigned int GetMaxFPS();

    bool ShouldClose();
    void SetShouldClose(bool should_close);

    static bool IsWindowValid(GLWindow *window);

    SDL_GLContext CreateContext();
    bool MakeContextCurrent(SDL_GLContext ctx);

    bool ClearWindow();
    bool SwapWindow();

    float GetWaitTime(float dt);

    SDL_Window *GetWindow();

private:
    SDL_Window *window;
    const char *title;
    unsigned int width, height, max_fps;
    bool running;
};

#endif