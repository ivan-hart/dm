#include <glad/glad.h>
#include <SDL2/SDL.h>
#include <SDL2/SDL_opengl.h>
#include <SDL2/SDL_image.h>
#include <glm/ext.hpp>

#include <iostream>
#include <vector>
#include <string>

#define WINDOW_TITLE "Nomad's Doom"
#define WINDOW_W 1280
#define WINDOW_H 720
#define MAX_FPS 60

bool mouseCaptured = false;
int lastMouseX = WINDOW_W / 2;
int lastMouseY = WINDOW_H / 2;
float mouseSensitivity = 0.1f;

SDL_Window *window;
SDL_GLContext glContext;

const int gridSize = 1000;
const float gridSpacing = 1.0f / (gridSize - 1);

std::vector<GLfloat> vertices;
std::vector<GLuint> indices;

const char *vert_shader = R"(
#version 460 core

layout (location = 0) in vec3 aPos;

uniform mat4 projection;
uniform mat4 view;
uniform mat4 model;
uniform float time;

out float height;

void main() {
    vec3 pos = aPos;
    pos.y = sin(pos.x * 10 + time) * 0.1 + sin(pos.z * 8 + time * 0.7) * 0.15;
    height = pos.y;
    gl_Position = projection * view * model * vec4(pos, 1.0f);
}
)";

const char *frag_shader = R"(
#version 460 core

in float height;
out vec4 FragColor;

void main() {
    vec3 color = mix(vec3(0.2, 0.6, 0.1), vec3(0.7, 0.7, 0.5), smoothstep(0.0, 0.2, height));
    FragColor = vec4(color, 1.0f);
}
)";

// int level[10][10] = {
//     {1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
//     {1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
//     {1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
//     {1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
//     {1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
//     {1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
//     {1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
//     {1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
//     {1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
//     {1, 1, 1, 1, 1, 1, 1, 1, 1, 1}};

int main()
{
    if (SDL_Init(SDL_INIT_EVERYTHING) > 0)
    {
        std::cout << "woops\n";
    }

    window = SDL_CreateWindow(WINDOW_TITLE, SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, WINDOW_W, WINDOW_H, SDL_WINDOW_SHOWN | SDL_WINDOW_OPENGL);
    if (window == nullptr)
    {
        std::cout << "Window go comblumy\n";
        SDL_Quit();
    }

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 4);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 6);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    glContext = SDL_GL_CreateContext(window);
    if (!glContext)
    {
        std::cout << "Context go poop\n";
        SDL_DestroyWindow(window);
        SDL_Quit();
    }

    if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress))
    {
        std::cout << "Glad no worky\n";
        SDL_GL_DeleteContext(glContext);
        SDL_DestroyWindow(window);
        SDL_Quit();
    }

    for (int z = 0; z < gridSize; ++z)
    {
        for (int x = 0; x < gridSize; ++x)
        {
            float xPos = x * gridSpacing - 0.5f;
            float zPos = z * gridSpacing - 0.5f;
            vertices.push_back(xPos);
            vertices.push_back(0.0f); // Y will be modified in the vertex shader
            vertices.push_back(zPos);
        }
    }

    for (int z = 0; z < gridSize - 1; ++z)
    {
        for (int x = 0; x < gridSize - 1; ++x)
        {
            GLuint topLeft = z * gridSize + x;
            GLuint topRight = topLeft + 1;
            GLuint bottomLeft = (z + 1) * gridSize + x;
            GLuint bottomRight = bottomLeft + 1;

            indices.push_back(topLeft);
            indices.push_back(bottomLeft);
            indices.push_back(topRight);

            indices.push_back(topRight);
            indices.push_back(bottomLeft);
            indices.push_back(bottomRight);
        }
    }

    glClearColor(0.2f, 0.4f, 0.8f, 1.0f);
    glViewport(0, 0, WINDOW_W, WINDOW_H);
    glEnable(GL_DEPTH_TEST);

    glm::mat4 projection, view, model;
    projection = glm::perspective(glm::radians(60.0f), (float)WINDOW_W / WINDOW_H, 0.001f, 1000.0f);
    model = glm::scale(glm::mat4(1.0f), glm::vec3(100.0f, 3.0f, 100.0f));

    glm::vec3 cameraPos = glm::vec3(0.0f, 2.0f, -2.0f);
    glm::vec3 cameraFront = glm::vec3(0.0f, 0.0f, -1.0f);
    glm::vec3 cameraUp = glm::vec3(0.0f, 1.0f, 0.0f);
    float yaw = -90.0f;   // Yaw is initialized to -90.0 degrees since a yaw of 0.0 results in a direction vector pointing to the right
    float pitch = -45.0f; // Pitch is initialized to -45.0 degrees to look downwards
    view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

    GLuint VAO, VBO, EBO, shader;
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);

    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(GLfloat), vertices.data(), GL_STATIC_DRAW);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GL_FLOAT), (void *)0);
    glEnableVertexAttribArray(0);

    glBindVertexArray(0);

    GLuint vshader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vshader, 1, &vert_shader, NULL);
    glCompileShader(vshader);

    GLuint fshader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fshader, 1, &frag_shader, NULL);
    glCompileShader(fshader);

    shader = glCreateProgram();
    glAttachShader(shader, vshader);
    glAttachShader(shader, fshader);
    glLinkProgram(shader);

    glDeleteShader(vshader);
    glDeleteShader(fshader);

    bool running = true;

    SDL_SetRelativeMouseMode(SDL_FALSE);
    Uint32 LAST, NOW = SDL_GetPerformanceCounter();
    float dt, waitTime, frameTime;
    while (running)
    {
        SDL_Event e;
        while (SDL_PollEvent(&e))
        {
            if (e.type == SDL_QUIT)
            {
                running = false;
            }
            else if (e.type == SDL_KEYDOWN)
            {
                if (e.key.keysym.sym == SDLK_TAB)
                {
                    mouseCaptured = !mouseCaptured;
                    SDL_SetRelativeMouseMode(mouseCaptured ? SDL_TRUE : SDL_FALSE);
                }
            }
            else if (e.type == SDL_MOUSEMOTION && mouseCaptured)
            {
                float xoffset = e.motion.xrel * mouseSensitivity;
                float yoffset = -e.motion.yrel * mouseSensitivity; // Reversed since y-coordinates go from bottom to top

                yaw += xoffset;
                pitch += yoffset;

                // Make sure that when pitch is out of bounds, screen doesn't get flipped
                if (pitch > 89.0f)
                    pitch = 89.0f;
                if (pitch < -89.0f)
                    pitch = -89.0f;

                glm::vec3 front;
                front.x = cos(glm::radians(yaw)) * cos(glm::radians(pitch));
                front.y = sin(glm::radians(pitch));
                front.z = sin(glm::radians(yaw)) * cos(glm::radians(pitch));
                cameraFront = glm::normalize(front);
            }
        }

        LAST = NOW;
        NOW = SDL_GetPerformanceCounter();
        dt = (float)(NOW - LAST) / SDL_GetPerformanceFrequency();
        frameTime = (float)(1 / MAX_FPS) * 1000;
        waitTime = dt - frameTime;
        std::cout << waitTime << std::endl;

        const Uint8 *keystate = SDL_GetKeyboardState(NULL);

        float cameraSpeed = 5.0f * dt;

        if (keystate[SDL_SCANCODE_W])
        {
            cameraPos += cameraSpeed * cameraFront;
        }
        if (keystate[SDL_SCANCODE_S])
        {
            cameraPos -= cameraSpeed * cameraFront;
        }
        if (keystate[SDL_SCANCODE_A])
        {
            cameraPos -= glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
        }
        if (keystate[SDL_SCANCODE_D])
        {
            cameraPos += glm::normalize(glm::cross(cameraFront, cameraUp)) * cameraSpeed;
        }

        // Update view matrix
        view = glm::lookAt(cameraPos, cameraPos + cameraFront, cameraUp);

        // Rest of your rendering code...
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        glUseProgram(shader);

        glUniformMatrix4fv(glGetUniformLocation(shader, "projection"), 1, GL_FALSE, glm::value_ptr(projection));
        glUniformMatrix4fv(glGetUniformLocation(shader, "view"), 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(glGetUniformLocation(shader, "model"), 1, GL_FALSE, glm::value_ptr(model));
        glUniform1f(glGetUniformLocation(shader, "time"), SDL_GetTicks() / 1000.0f);

        glBindVertexArray(VAO);
        glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
        glBindVertexArray(0);
        glUseProgram(0);

        SDL_GL_SwapWindow(window);
        SDL_Delay(waitTime);
    }
}