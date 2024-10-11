#include "projection.h"

//
//
//  Returns an instance of Projection to be used as a singleton in the rest of the application
//  this is so that we dont have to do a bunch of weird stuff with arguments
//
//
Projection &Projection::GetInstance()
{
    static Projection instance;
    return instance;
}

//
//
//  returns the projection as a 4x4 float matrix
//
//
glm::mat4 Projection::GetProjection()
{
    return this->projection;
}

//
//
//  sets the current projection as a 4x4 float matrix afer checking the input
//  to prevent zerioing of the projection
//
//
void Projection::SetProjection(glm::mat4 projection)
{
    if(projection == glm::mat4(0.0f))
    {
        std::cout << "Projection cannot be zero\n";
    }
    this->projection = projection;
}
