#include "ClickListener.h"
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
        Rocket::Core::Context* context = event.GetTargetElement()->GetContext();
        Rocket::Core::String shader_name = event.GetTargetElement()->GetInnerRML();
        std::string c_shader_name = shader_name.CString();

        /*
        if(c_shader_name == "Vertex")
        {
            std::string shader_source = renderer::Renderer::instance()->getShaderSourceOf(renderer::GLSL::VERTEX);
            Rocket::Core::String code = shader_source.c_str();
            context->GetDocument("main")->GetElementById("content")->SetInnerRML(code.CString());
        }
        if(c_shader_name == "Fragment")
        {
            std::string shader_source = renderer::Renderer::instance()->getShaderSourceOf(renderer::GLSL::FRAGMENT);
            Rocket::Core::String code = shader_source.c_str();
            context->GetDocument("main")->GetElementById("content")->SetInnerRML(code.CString());
        }
        if(c_shader_name == "Tesselation")
        {
            std::string shader_source = renderer::Renderer::instance()->getShaderSourceOf(renderer::GLSL::TESS_CONTROL);
            Rocket::Core::String code = shader_source.c_str();
            context->GetDocument("main")->GetElementById("content")->SetInnerRML(code.CString());
        }
        if(c_shader_name == "Geometry")
        {
            std::string shader_source = renderer::Renderer::instance()->getShaderSourceOf(renderer::GLSL::GEOMETRY);
            Rocket::Core::String code = shader_source.c_str();
            context->GetDocument("main")->GetElementById("content")->SetInnerRML(code.CString());
        }*/

        shader_name.Append(" Shader");
        context->GetDocument("main")->GetElementById("title")->SetInnerRML(shader_name);
    }
}
