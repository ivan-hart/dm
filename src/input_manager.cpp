#include "input_manager.h"

//
//
//  Contains multiple definitions for an abstraction 
//  layer for input management
//
//
namespace Input
{
    const SDL_Scancode W = SDL_SCANCODE_W;
    const SDL_Scancode S = SDL_SCANCODE_S;
    const SDL_Scancode D = SDL_SCANCODE_D;
    const SDL_Scancode A = SDL_SCANCODE_A;
    const SDL_Scancode E = SDL_SCANCODE_E;
    const SDL_Scancode ESCAPE = SDL_SCANCODE_ESCAPE;
    const SDL_Scancode SPACE = SDL_SCANCODE_SPACE;
}

//
//
//  Returns the instance of the InputManager as a singleton
//
//
InputManager& InputManager::GetInstance()
{
    static InputManager instance;
    return instance;
}

//
//
//  Adds the input as a string and and a scancode pair into the input map
//
//
void InputManager::AddInput(const std::string& input_name, SDL_Scancode scancode)
{
    inputMap[input_name] = scancode;
}

//
//
//  Checks to see if the input of the selected type from the param is on
//
//
bool InputManager::IsInputPressed(const std::string& input_name) const
{
    auto it = inputMap.find(input_name);
    if (it != inputMap.end()) {
        return keyboardState[it->second] == 1;
    }
    return false;
}

//
//
//  Removes the selected input from the input map using a string as the key
//
//
void InputManager::RemoveInput(const std::string& input_name)
{
    inputMap.erase(input_name);
}

//
//
//  Called every frame in the application to update the keyboard state
//
//
void InputManager::PollKeyEvents()
{
    keyboardState = SDL_GetKeyboardState(NULL);
}
