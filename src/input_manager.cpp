#include "input_manager.h"

namespace Input
{
    const SDL_Scancode W = SDL_SCANCODE_W;
    const SDL_Scancode S = SDL_SCANCODE_S;
    const SDL_Scancode D = SDL_SCANCODE_D;
    const SDL_Scancode A = SDL_SCANCODE_A;
    const SDL_Scancode SPACE = SDL_SCANCODE_SPACE;
    const SDL_Scancode ESCAPE = SDL_SCANCODE_ESCAPE;
}

InputManager& InputManager::GetInstance()
{
    static InputManager instance;
    return instance;
}

void InputManager::AddInput(const std::string& input_name, SDL_Scancode scancode)
{
    inputMap[input_name] = scancode;
}

bool InputManager::IsInputPressed(const std::string& input_name) const
{
    auto it = inputMap.find(input_name);
    if (it != inputMap.end()) {
        return keyboardState[it->second] == 1;
    }
    return false;
}

void InputManager::RemoveInput(const std::string& input_name)
{
    inputMap.erase(input_name);
}

void InputManager::PollKeyEvents()
{
    keyboardState = SDL_GetKeyboardState(NULL);
}
