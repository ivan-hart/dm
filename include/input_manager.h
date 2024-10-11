#ifndef INPUT_MANAGER_H
#define INPUT_MANAGER_H

#include <SDL2/SDL.h>
#include <iostream>
#include <vector>
#include <map>

namespace Input
{
    extern const SDL_Scancode W;
    extern const SDL_Scancode S;
    extern const SDL_Scancode D;
    extern const SDL_Scancode A;
    extern const SDL_Scancode SPACE;
    extern const SDL_Scancode ESCAPE;
};

class InputManager
{
    public:
        static InputManager& GetInstance();

        void AddInput(const std::string& input_name, SDL_Scancode scancode);
        bool IsInputPressed(const std::string& input_name) const;
        void RemoveInput(const std::string& input_name);

        void PollKeyEvents();

    private:
        InputManager() {} // Private constructor for singleton
        std::map<std::string, SDL_Scancode> inputMap;
        const Uint8* keyboardState;

        // Delete copy constructor and assignment operator
        InputManager(const InputManager&) = delete;
        InputManager& operator=(const InputManager&) = delete;
};

#endif
