#include <Rocket/Core.h>
#include <sstream>

#include "ClickListener.h"

static ClickListener m_click_listener;
static Rocket::Core::Context *m_context;
static renderer::GLSL::GLSLShaderType m_shaderType;
static std::string m_shader_name;
static std::string m_shader_source;

void ClickListener::RegisterClickableContainer(Rocket::Core::Element* element)
{
	element->AddEventListener("click", &m_click_listener);
	std::cout << "LibRocket: Registered click event listener @ element with id = " << element->GetId().CString() << std::endl;
}

void ClickListener::ProcessEvent(Rocket::Core::Event& event)
{
    if(event == "click")
    {
		std::cout << "CLICKED" << std::endl;

		Rocket::Core::Element *l_element = event.GetTargetElement();
		Rocket::Core::String l_id = l_element->GetId();
		
		m_context = l_element->GetContext();

		/*
        Rocket::Core::Element* element = event.GetTargetElement();
		Rocket::Core::String element_tag_name = element->GetTagName();
        //c_shader_name = shader_name.CString();
		//std::cout << c_shader_name << std::endl;
		std::cout << element_tag_name.CString() << std::endl;
		*/

        if(l_id == "vs")
        {
			m_shader_name = "Vertex";
			m_shaderType = renderer::GLSL::VERTEX;
			
        }
        if(l_id == "fs")
        {
			m_shader_name = "Fragment";
			m_shaderType = renderer::GLSL::FRAGMENT;
		}
		if (l_id == "gs")
		{
			m_shader_name = "Geometry";
			m_shaderType = renderer::GLSL::GEOMETRY;
		}
		if (l_id == "ts")
		{
			m_shader_name = "Tesselation";
			m_shaderType = renderer::GLSL::TESS_CONTROL;
		}

		// Set shader title
		m_shader_name.append(" Shader");
		m_context->GetDocument("main")->GetElementById("title")->SetInnerRML(m_shader_name.c_str());
		// Set shader source
		unsigned int lineCount;
		m_shader_source = renderer::Renderer::instance()->getShaderSourceOf(m_shaderType, lineCount);
		Rocket::Core::String code = m_shader_source.c_str();
		m_context->GetDocument("main")->GetElementById("wrapper")->GetElementById("content")->SetInnerRML(code.CString());
		// Fill line numbers
		std::string numbers = "";
		for (unsigned int i = 0; i < 50; ++i)
		{
			std::string stringNumber = static_cast<ostringstream*>(&(ostringstream() << i))->str();
			numbers += stringNumber + "\n";
		}
		std::cout << numbers  << std::endl;

		m_context->GetDocument("main")->GetElementById("wrapper")->GetElementById("lineNumbers")->SetInnerRML(numbers.c_str());
    }
	return;
}
