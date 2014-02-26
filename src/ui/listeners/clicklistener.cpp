#include "clicklistener.h"
#include <Rocket/Core.h>


static ClickListener click_listener;

void ClickListener::RegisterClickableContainer(Rocket::Core::Element* element)
{
    element->AddEventListener("click", &click_listener);
}

void ClickListener::ProcessEvent(Rocket::Core::Event& event)
{
    if(event == "click")
    {
        Rocket::Core::String shader_name = event.GetTargetElement()->GetInnerRML();

        Rocket::Core::Context* context = event.GetTargetElement()->GetContext();
        context->GetDocument("main")->GetElementById("title")->SetInnerRML(shader_name);

		std::cout << "CLICK" << std::endl;
    }
}
