#include "view.h"

//
//
//  Returns a singleton so that we dont have to do a bunch of weird stuff with function calls
//
//
View &View::GetInstance()
{
    static View instance;
    return instance;
}

//
//
//  returns the view as a 4x4 matrix
//
//
glm::mat4 View::GetView()
{
    return this->view;
}

//
//
//  sets the current view as long as its not zero
//
//
void View::SetView(glm::mat4 view)
{
    if(view == glm::mat4(0.0f))
    {
        std::cout << "View cannot be zero\n";
    }
    this->view = view;
}
