#ifndef GAME_H
#define GAME_H

#include <window.h>
#include <input_manager.h>
#include <projection.h>
#include <view.h>

#include <glad/glad.h>
#include <SDL2/SDL.h>
#include <glm/ext.hpp>

#include <iostream>

//
//
//  Entry point of the application that manages the game objects and thread management
//
//
class Game
{
public:
    Game();
    ~Game();

private:
    GLWindow *window;
    SDL_GLContext ctx;
    InputManager * inputManager;
    View * view;
    Projection * projection;

    bool InitSDL();
    bool InitWindow();
    bool InitGL();
    bool InitGLRules();
    bool InitGameObjects();
    bool Init();
    void PollEvents();
    void PollKeyEvents();
    void Update(float dt);
    void Render();
    void MainLoop();
    void Run();
    void Cleanup();
};

#endif