#ifndef PROJECTION_H
#define PROJECTION_H

#include <glad/glad.h>
#include <glm/ext.hpp>

#include <iostream>

class Projection
{
    public:
        static Projection& GetInstance();

        glm::mat4 GetProjection();
        void SetProjection(glm::mat4 projection);
    private:
        Projection() {};

        glm::mat4 projection;

        Projection(const Projection&) = delete;
        Projection& operator=(const Projection&) = delete;
};

#endif