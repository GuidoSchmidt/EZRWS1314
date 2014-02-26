#ifndef CLICKLISTENER_H
#define CLICKLISTENER_H

#include <Rocket/Core/EventListener.h>
#include <Rocket/Core/Types.h>
#include <iostream>

class ClickListener : public Rocket::Core::EventListener
{
    public:
        /// Registers an elemenet as being a container of draggable elements.
        static void RegisterClickableContainer(Rocket::Core::Element* element);

    protected:
        virtual void ProcessEvent(Rocket::Core::Event& event);
};

#endif // CLICKLISTENER_H
