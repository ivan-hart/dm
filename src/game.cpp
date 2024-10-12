#include "game.h"

//
//
//  The primary contructor for the Game class
//  When an object of type Game is created run 
//  will be called each time
//
//
Game::Game()
{
    Run();
}


//
//
//  The primary destructor for the Game class
//  When a object of type Game reaches the end 
//  of its scope Game::Cleanup() will be called
//
//
Game::~Game()
{
    Cleanup();
}

//
//
//  Initalizes SDL and returns weather it was successful
//
//
bool Game::InitSDL()
{
    return SDL_Init(SDL_INIT_EVERYTHING) <= 0;
}

//
//
//  Initalizes SDL2_image
//
//
bool Game::InitSDLImage()
{
    return IMG_Init(IMG_INIT_PNG) & IMG_INIT_PNG;
}

//
//
//  Initalizes the main window and check if it is valid
//
//
bool Game::InitWindow()
{
    window = new GLWindow("Doom Like", CENTERED, CENTERED, 1280, 720, SHOWN | OPENGL);
    return GLWindow::IsWindowValid(window);
}

//
//
//  Initalizes OpenGL and creates a gl context
//
//
bool Game::InitGL()
{
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    this->ctx = window->CreateContext();
    window->MakeContextCurrent(this->ctx);
    return gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress);
}

//
//
//  Initalizes various stuff for OpenGL to work in a 3d environment
//
//
bool Game::InitGLRules()
{
    glClearColor(0.2f, 0.3f, 0.8f, 1.0f);
    glViewport(0, 0, window->GetWindowWidth(), window->GetWindowHeight());
    glEnable(GL_DEPTH_TEST);

    GLenum error = glGetError();
    if(error != GL_NO_ERROR)
    {
        std::cout << "Error initalizing gl rules: " << error << std::endl;
        return false;
    }
    return true;
}

//
//
//  Initalies various game objects such as the Player and Level manager
//
//
bool Game::InitGameObjects()
{
    bool success = true;

    inputManager = &InputManager::GetInstance();
    inputManager->AddInput("left", SDL_SCANCODE_A);
    inputManager->AddInput("escape", SDL_SCANCODE_ESCAPE);

    projection = &Projection::GetInstance();
    projection->SetProjection(glm::perspective(glm::radians(60.0f), (float)window->GetWindowWidth() / window->GetWindowHeight(), 0.01f, 1000.0f));

    view = &View::GetInstance();

    return success;
}

//
//
//  Calls all the init functions to initalize most of the game
//  and ehcks for errors with each step
//
//
bool Game::Init()
{
    bool success = true;

    if (!InitSDL())
    {
        success = false;
        std::cout << "Error initalizing SDL: " << SDL_GetError() << std::endl;
    } else {
        std::cout << "SDL initialized\n";
    }

    if (!InitSDLImage())
    {
        success = false;
        std::cout << "Error initalizing SDL2_image " << IMG_GetError() << std::endl;

    } else {
        std::cout << "Initalized SDL2 Image\n";
    }

    if (!InitWindow())
    {
        success = false;
        std::cout << "Erorr initalizing the window: " << SDL_GetError() << std::endl;
    } else {
        std::cout << "Window created\n";
    }

    if (!InitGL())
    {
        success = false;
        std::cout << "Error initlizing OpenGL: " << glGetError() << std::endl;
    } else {
        std::cout << "Initalized GL\n";
    }

    if(!InitGLRules())
    {
        success = false;
        std::cout << "Error initalizing gl rules\n";
    } else {
        std::cout << "Initialized gl rules\n";
    }

    if (!InitGameObjects())
    {
        success = false;
        std::cout << "Error initalizing game objects\n";
    }

    return success;
}

//
//
//  Polls various events inside the application 
//  for managing the life of the application
//
//
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

//
//
//  Uses the InputManager class to check for 
//  inputs regarding the life of the application
//
//
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

//
//
//  Updates the various game obejcts or calls 
//  their own Update(float) functions
//
//
void Game::Update(float dt)
{
}

//
//
//  Renders the various game objects or call 
//  their own Render function
//
//
void Game::Render()
{
}

//
//
//  Calls various functions to update game 
//  objects and the window and draws stuff 
//  while also calculating the delta v and 
//  limiting the frame frate
//
//
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
        window->ClearWindow();
        Render();
        window->SwapWindow();
        SDL_Delay(window->GetWaitTime(dt));
    }
}

//
//
//  Calls the Init function and returns if the 
//  application fails to initalize if it initalizes
//  it calls the MainLoop function
//
//
void Game::Run()
{
    if (!Init())
        return;

    MainLoop();
}

//
//
//  Frees various objects from memory
//  And quits out of the appication
//
//
void Game::Cleanup()
{
    std::cout << "Cleaning the application\n";
    if (window != NULL)
    {
        std::cout << "Freeing the window\n";
        delete window;
    }
    
    if (ctx)
    {
        std::cout << "Deleteing the GL context\n";
        SDL_GL_DeleteContext(ctx);
    }

    std::cout << "Quiting SDL\n";
    SDL_Quit();
}
