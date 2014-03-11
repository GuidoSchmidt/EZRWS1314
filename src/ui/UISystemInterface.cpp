#include "UISystemInterface.h"

float UISystemInterface::GetElapsedTime()
{
    return static_cast<float>(glfwGetTime());
}
