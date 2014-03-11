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

        std::string c_shader_name = shader_name.CString();

        if(c_shader_name == "Vertex")
        {
            std::string shader_source = renderer::Renderer::instance()->getShaderSourceOf(renderer::GLSL::VERTEX);
            Rocket::Core::String code = shader_source.c_str();

            std::string test = "TEST";
            test.append(shader_source.c_str());
            context->GetDocument("main")->GetElementById("content")->SetInnerRML(code.CString());
        }
        if(c_shader_name == "Fragment")
        {
            std::cout << c_shader_name << std::endl;
            context->GetDocument("main")->GetElementById("content")->SetInnerRML(static_cast<Rocket::Core::String>("F"));
        }
        if(c_shader_name == "Tesselation")
        {
            std::cout << c_shader_name << std::endl;
            context->GetDocument("main")->GetElementById("content")->SetInnerRML(static_cast<Rocket::Core::String>("T"));
        }
        if(c_shader_name == "Geometry")
        {
            std::cout << c_shader_name << std::endl;
            context->GetDocument("main")->GetElementById("content")->SetInnerRML(static_cast<Rocket::Core::String>("G"));
        }
    }
}
