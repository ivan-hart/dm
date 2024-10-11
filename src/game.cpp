#include "game.h"

Game::Game()
{
    Run();
}

Game::~Game()
{
    Cleanup();
}

bool Game::InitSDL()
{
    return SDL_Init(SDL_INIT_EVERYTHING) <= 0;
}

bool Game::InitWindow()
{
    window = new GLWindow("Doom Like", CENTERED, CENTERED, 1280, 720, SHOWN | OPENGL);
    return GLWindow::IsWindowValid(window);
}

bool Game::InitGL()
{
    this->ctx = window->CreateContext();
    window->MakeContextCurrent(this->ctx);
    return gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress);
}

bool Game::InitGameObjects()
{
    bool success = true;

    inputManager = &InputManager::GetInstance();
    inputManager->AddInput("left", SDL_SCANCODE_A);
    inputManager->AddInput("escape", SDL_SCANCODE_ESCAPE);

    return success;
}

bool Game::Init()
{
    bool success = true;

    if (!InitSDL())
    {
        success = false;
        std::cout << "Error initalizing SDL: " << SDL_GetError() << std::endl;
    }

    if (!InitWindow())
    {
        success = false;
        std::cout << "Erorr initalizing the window: " << SDL_GetError() << std::endl;
    }

    if (!InitGL())
    {
        success = false;
        std::cout << "Error initlizing OpenGL: " << glGetError() << std::endl;
    }

    if (!InitGameObjects())
    {
        success = false;
        std::cout << "Error initalizing game objects\n";
    }

    return success;
}

void Game::PollEvents()
{
    SDL_Event e;
    while (SDL_PollEvent(&e))
    {
        switch (e.type)
        {
        case SDL_QUIT:
            if (GLWindow::IsWindowValid(window))
                window->SetShouldClose(true);
        }
    }
}

void Game::PollKeyEvents()
{
    inputManager->PollKeyEvents();
    if(inputManager->IsInputPressed("left"))
    {
        std::cout << "left pressed\n";
    }
    if(inputManager->IsInputPressed("escape"))
    {
        window->SetShouldClose(true);
    }
}

void Game::Update(float dt)
{
}

void Game::Render()
{
}

void Game::MainLoop()
{
    if (window == nullptr)
    {
        std::cout << "Window is null\n";
        return;
    }

    Uint64 NOW = SDL_GetPerformanceCounter();
    Uint64 LAST = NOW;
    float dt;

    while (!window->ShouldClose())
    {
        LAST = NOW;
        NOW = SDL_GetPerformanceCounter();
        dt = (float)(NOW - LAST) / SDL_GetPerformanceFrequency();
        PollEvents();
        PollKeyEvents();
        Update(dt);
        Render();
        SDL_Delay(16);
    }
}

void Game::Run()
{
    if (!Init())
        return;

    MainLoop();
}

void Game::Cleanup()
{
    if (window != NULL)
        delete window;
    
    if (ctx)
        SDL_GL_DeleteContext(ctx);

    SDL_Quit();
}
