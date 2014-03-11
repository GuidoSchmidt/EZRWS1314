#include "./utils/Common.h"
#include "./renderer/Context.h"
#include "./renderer/Renderer.h"
#include <Rocket/Core.h>
#include <Rocket/Debugger.h>
#include "./ui/UISystemInterface.h"
#include "./ui/shell/Shell.h"
#include "./ui/shell/Input.h"

#include "./ui/listeners/ClickListener.h"
#include <string>

Rocket::Core::Context *context = 0;
extern GLFWwindow *glfwindow;

void GameLoop()
{

}

int main(void)
{
    if(!glfwInit())
    {
        std::cout << "ERROR: GLFW initialization failed!" << std::endl;
    }
    else {
        std::cout << "GLFW initalized!" << std::    endl;
    }

    // Generic OS initialisation, creates a window and attaches OpenGL.
    Shell::Initialise("../Samples/tutorial/template/");
    Shell::OpenWindow("Template Tutorial", true);
	glfwMakeContextCurrent(glfwindow);

    if(glewInit() != GLEW_OK)
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
    opengl_renderer.SetViewport(1024,768);

    Rocket::Core::Initialise();

    context = Rocket::Core::CreateContext("main", Rocket::Core::Vector2i(1024, 768));
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
    Rocket::Core::ElementDocument* textBox = context->LoadDocument(RESOURCES_PATH "/ui/shaderUI/mainWindow.rml");
    Rocket::Core::ElementDocument* navBar = context->LoadDocument(RESOURCES_PATH "/ui/shaderUI/navBar.rml");

    if (textBox != 0 || navBar != 0)
    {
        textBox->Show();

        navBar->Show();
        navBar->SetProperty("left", Rocket::Core::Property(0, Rocket::Core::Property::PX));
        navBar->SetProperty("top", Rocket::Core::Property(0, Rocket::Core::Property::PX));
    }
    ClickListener::RegisterClickableContainer(navBar->GetElementById("vertex"));
    ClickListener::RegisterClickableContainer(navBar->GetElementById("geometry"));
    ClickListener::RegisterClickableContainer(navBar->GetElementById("tesselation"));
    ClickListener::RegisterClickableContainer(navBar->GetElementById("fragment"));

	renderer::Renderer* main_renderer;
	main_renderer = renderer::Renderer::instance();
	main_renderer->init(glfwindow);
	main_renderer->setupRenderer(glfwindow);

	while (!glfwWindowShouldClose(glfwindow))
    {
		glEnable(GL_DEPTH_TEST);
		renderer::Renderer::instance()->renderloop(glfwindow);
	

		// LibRocket input handling
		int x = 0, y = 0;
		int key_modifier_state = -1;
		double xpos, ypos;
		glfwGetCursorPos(glfwindow, &xpos, &ypos);
		context->ProcessMouseMove(static_cast<int>(xpos), static_cast<int>(ypos), key_modifier_state);

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

		glDisable(GL_DEPTH_TEST);
		glDisable(GL_TEXTURE_2D);
		
		glClearColor(0, 0, 0, 1);
		glEnableClientState(GL_VERTEX_ARRAY);
		glEnableClientState(GL_COLOR_ARRAY);

		glEnable(GL_BLEND);
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		glOrtho(0, 1024, 768, 0, -1, 1);

		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
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
