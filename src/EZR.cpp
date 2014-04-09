#include <string>
#include <sstream>
#include <algorithm>
#include <Rocket/Core.h>
#include <Rocket/Debugger.h>

#include "./utils/Common.h"
#include "./renderer/Renderer.h"
#include "./ui/UISystemInterface.h"
#include "./ui/shell/Shell.h"
#include "./ui/shell/Input.h"
#include "./ui/listeners/ClickListener.h"


Rocket::Core::Context *context = 0;
extern GLFWwindow *glfwindow;
extern double scroll;
extern glm::ivec2 size;
static float m_contentXPos = size.x - 20;

int main(void)
{
	if (!glfwInit())
	{
		std::cout << "ERROR: GLFW initialization failed!" << std::endl;
	}
	else {
		std::cout << "GLFW initalized!" << std::endl;
	}

	// Generic OS initialisation, creates a window and attaches OpenGL.
	Shell::Initialise("../Samples/tutorial/template/");
	Shell::OpenWindow("Template Tutorial", true);
	glfwMakeContextCurrent(glfwindow);

	if (glewInit() != GLEW_OK)
	{
		//! Error handling
		std::cerr << "ERROR (GLEW): Initialization failed!\n";
	}

	const GLubyte* glinfo_renderer_ptr = glGetString(GL_RENDERER);
	const GLubyte* glinfo_vendor_ptr = glGetString(GL_VENDOR);
	const GLubyte* glinfo_openglVersion_ptr = glGetString(GL_VERSION);
	const GLubyte* glinfo_glslVersion_ptr = glGetString(GL_SHADING_LANGUAGE_VERSION);

	GLint glinfo_major, glinfo_minor;
	glGetIntegerv(GL_MAJOR_VERSION, &glinfo_major);
	glGetIntegerv(GL_MINOR_VERSION, &glinfo_minor);

	//! Write renderer infos to log
	std::cout << "--- HARDWARE -------------------------------------------------------------" << std::endl;
	std::cout << "OpenGL Vendor:     ";
	std::cout << glinfo_vendor_ptr << std::endl;
	std::cout << "OpenGL Renderer:   ";
	std::cout << glinfo_renderer_ptr << std::endl;
	std::cout << "OpenGL Version:    ";
	std::cout << glinfo_openglVersion_ptr << std::endl;;
	std::cout << "GLSL Version:      ";
	std::cout << glinfo_glslVersion_ptr << std::endl;

	//! System interface
	UISystemInterface system_interface;
	Rocket::Core::SetSystemInterface(&system_interface);

	//! Render interface
	ShellRenderInterfaceOpenGL opengl_renderer;
	Rocket::Core::SetRenderInterface(&opengl_renderer);
	opengl_renderer.SetViewport(size.x, size.y);

	Rocket::Core::Initialise();

	context = Rocket::Core::CreateContext("main", Rocket::Core::Vector2i(size.x, size.y));
	if (context == NULL)
	{
		Rocket::Core::Shutdown();
		glfwTerminate();
		return -1;
	}

	// Load and show the demo document.
	Rocket::Debugger::Initialise(context);
	Input::SetContext(context);

	Shell::LoadFonts(RESOURCES_PATH "/ui/fonts/");

	// Load and show the tutorial document.
	Rocket::Core::ElementDocument* contentBox = context->LoadDocument(RESOURCES_PATH "/ui/shaderUI/mainWindow.rml");
	Rocket::Core::ElementDocument* navBar = context->LoadDocument(RESOURCES_PATH "/ui/shaderUI/navBar.rml");

	if (contentBox != 0 || navBar != 0)
	{
		contentBox->SetProperty("left", Rocket::Core::Property(m_contentXPos, Rocket::Core::Property::PX));
		contentBox->Show();

		navBar->SetProperty("left", Rocket::Core::Property(0, Rocket::Core::Property::PX));
		navBar->SetProperty("top", Rocket::Core::Property(0, Rocket::Core::Property::PX));
		navBar->SetProperty("top", Rocket::Core::Property(0, Rocket::Core::Property::PX));
		navBar->Show();
	}
	//! Fill line numbers
	std::string numbers = "";
	for (unsigned int i = 0; i < 500; ++i)
	{
		std::string stringNumber = static_cast<ostringstream*>(&(ostringstream() << i))->str();
		numbers += stringNumber + "<br />";
	}
	contentBox->GetElementById("wrapper")->GetElementById("lineNumbers")->SetInnerRML(numbers.c_str());
	
	ClickListener::RegisterClickableContainer(navBar->GetElementById("vs"));
	ClickListener::RegisterClickableContainer(navBar->GetElementById("ts"));
	ClickListener::RegisterClickableContainer(navBar->GetElementById("gs"));
	ClickListener::RegisterClickableContainer(navBar->GetElementById("fs"));

	renderer::Renderer* main_renderer;
	main_renderer = renderer::Renderer::instance();
	main_renderer->init(glfwindow);
	main_renderer->setupRenderer(glfwindow);

	while (!glfwWindowShouldClose(glfwindow))
	{
		//--- EZR rendering ------------------------------------------------------------------------
		glEnable(GL_DEPTH_TEST);

		renderer::Renderer::instance()->renderloop(glfwindow);

		//--- libRocket ----------------------------------------------------------------------------
		// LibRocket input handling
		int x = 0;
		int y = 0;
		int key_modifier_state = -1;
		double xpos, ypos;
		glfwGetCursorPos(glfwindow, &xpos, &ypos);
		context->ProcessMouseMove(static_cast<int>(xpos), static_cast<int>(ypos), key_modifier_state);
		context->ProcessMouseWheel((int)scroll, key_modifier_state);

		int button_index = 1;
		if (glfwGetMouseButton(glfwindow, GLFW_MOUSE_BUTTON_1) == GLFW_PRESS ||
			glfwGetMouseButton(glfwindow, GLFW_MOUSE_BUTTON_2) == GLFW_PRESS ||
			glfwGetMouseButton(glfwindow, GLFW_MOUSE_BUTTON_3) == GLFW_PRESS)
		{
			//std::cout << "Mouse Press" << std::endl;
			if (glfwGetMouseButton(glfwindow, GLFW_MOUSE_BUTTON_1))
				button_index = 0;
			if (glfwGetMouseButton(glfwindow, GLFW_MOUSE_BUTTON_2))
				button_index = 2;
			if (glfwGetMouseButton(glfwindow, GLFW_MOUSE_BUTTON_3))
				button_index = 1;

			context->ProcessMouseButtonDown(button_index, key_modifier_state);

			if (glfwGetMouseButton(glfwindow, GLFW_MOUSE_BUTTON_1) == GLFW_RELEASE ||
				glfwGetMouseButton(glfwindow, GLFW_MOUSE_BUTTON_2) == GLFW_RELEASE ||
				glfwGetMouseButton(glfwindow, GLFW_MOUSE_BUTTON_3) == GLFW_RELEASE
				)
			{
				//std::cout << "Mouse Release" << std::endl;
				if (glfwGetMouseButton(glfwindow, GLFW_MOUSE_BUTTON_1))
					button_index = 0;
				if (glfwGetMouseButton(glfwindow, GLFW_MOUSE_BUTTON_2))
					button_index = 2;
				if (glfwGetMouseButton(glfwindow, GLFW_MOUSE_BUTTON_3))
					button_index = 1;

				context->ProcessMouseButtonUp(button_index, key_modifier_state);
			}
		}

		if (xpos > m_contentXPos)
		{
			m_contentXPos = int(size.x * 0.5);
			contentBox->SetProperty("left", Rocket::Core::Property(m_contentXPos, Rocket::Core::Property::PX));
			float pos = min(static_cast<int>(scroll), 32);
			contentBox->GetElementById("wrapper")->SetProperty("top", Rocket::Core::Property(pos, Rocket::Core::Property::PX));
		}
		else if (xpos < m_contentXPos && ypos > 35)
		{
			scroll = 32.0;
			m_contentXPos = (size.x * 0.9);
			contentBox->SetProperty("left", Rocket::Core::Property(m_contentXPos, Rocket::Core::Property::PX));
		}

		context->Update();
		context->Render();

		glfwSwapBuffers(glfwindow);
		glfwPollEvents();
	}

	// Shutdown Rocket.
	context->RemoveReference();
	Rocket::Core::Shutdown();

	Shell::CloseWindow();
	Shell::Shutdown();

	return 0;
}
