#include "ClickListener.h"
#include <Rocket/Core.h>


static ClickListener click_listener;

enum class Shader{ non, vertex, tesselation, geometry, fragment };

Shader selected = Shader::non;

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

        if(c_shader_name == "Vertex")
        {
            if(selected != Shader::vertex)
            {
                std::string shader_source = renderer::Renderer::instance()->getShaderSourceOf(renderer::GLSL::VERTEX);
                Rocket::Core::String code = shader_source.c_str();
                context->GetDocument("main")->GetElementById("content")->SetInnerRML(code.CString());
                context->GetDocument("main")->SetProperty("display", "block");
                selected = Shader::vertex;
            }
            else
            {
                context->GetDocument("main")->SetProperty("display", "none");
                selected = Shader::non;
            }
        }
        else if(c_shader_name == "Fragment")
        {
            if(selected != Shader::fragment)
            {
                std::string shader_source = renderer::Renderer::instance()->getShaderSourceOf(renderer::GLSL::FRAGMENT);
                Rocket::Core::String code = shader_source.c_str();
                context->GetDocument("main")->GetElementById("content")->SetInnerRML(code.CString());
                context->GetDocument("main")->SetProperty("display", "block");
                selected = Shader::fragment;
            }
            else
            {
                context->GetDocument("main")->SetProperty("display", "none");
                selected = Shader::non;
            }
        }
        else if(c_shader_name == "Tesselation")
        {
            if(selected != Shader::tesselation)
            {
                std::string shader_source = renderer::Renderer::instance()->getShaderSourceOf(renderer::GLSL::TESS_CONTROL);
                Rocket::Core::String code = shader_source.c_str();
                context->GetDocument("main")->GetElementById("content")->SetInnerRML(code.CString());
                context->GetDocument("main")->SetProperty("display", "block");
                selected = Shader::tesselation;
            }
            else
            {
                context->GetDocument("main")->SetProperty("display", "none");
                selected = Shader::non;
            }
        }
        else if(c_shader_name == "Geometry")
        {
            if(selected != Shader::geometry)
            {
                std::string shader_source = renderer::Renderer::instance()->getShaderSourceOf(renderer::GLSL::GEOMETRY);
                Rocket::Core::String code = shader_source.c_str();
                context->GetDocument("main")->GetElementById("content")->SetInnerRML(code.CString());
                context->GetDocument("main")->SetProperty("display", "block");
                selected = Shader::geometry;
            }
            else
            {
                context->GetDocument("main")->SetProperty("display", "none");
                selected = Shader::non;
            }
        }

        shader_name.Append(" Shader");
        context->GetDocument("main")->GetElementById("title")->SetInnerRML(shader_name);
    }
}
