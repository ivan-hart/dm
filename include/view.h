#ifndef VIEW_H
#define VIEW_H

#include <glad/glad.h>
#include <glm/ext.hpp>

#include <iostream>

class View
{
    public:
        static View& GetInstance();

        glm::mat4 GetView();
        void SetView(glm::mat4 view);
    private:
        View() {};

        glm::mat4 view;

        View(const View&) = delete;
        View& operator=(const View&) = delete;
};

#endif