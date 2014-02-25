#ifndef UISYSTEMINTERFACE_H
#define UISYSTEMINTERFACE_H

#include <Rocket/Core/SystemInterface.h>
#include <GLFW/glfw3.h>

class UISystemInterface : public Rocket::Core::SystemInterface
{
    public:
        virtual float GetElapsedTime();
};

#endif // UISYSTEMINTERFACE_H
