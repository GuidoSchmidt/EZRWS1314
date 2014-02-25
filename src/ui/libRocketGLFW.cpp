#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <Rocket/Core/SystemInterface.h>
#include <Rocket/Core/RenderInterface.h>
#include <Rocket/Core.h>
#include <Rocket/Debugger.h>

#include "shell/ShellRenderInterfaceOpenGL.h"
#include "shell/Shell.h"
#include "shell/Input.h"

//GLFWwindow* glfw_windows_handle = 0;
//bool running = false;
//Rocket::Core::Context* context = 0;

#ifndef H_MYSYSTEMINTERFACE
#define H_MYSYSTEMINTERFACE

//Rocket::Core::Context* context = 0;

#endif // H_MYSYSTEMINTERFACE

/*
 * void GameLoop()
{
    glClear(GL_COLOR_BUFFER_BIT);

    context->Update();
    context->Render();

    Shell::FlipBuffers();
}
*/

/*
int oldmain(void)
{
	if(!glfwInit())
	{
		std::cout << "ERROR: GLFW initialization failed!" << std::endl;
	}
	else {
        std::cout << "GLFW initalized!" << std::    endl;
	}

//	if(!glewInit())
//	{
//		std::cout << "ERROR: GLEW initalization failed!" << std::endl;
//    }
//	else {
//		std::cout << "GLEW initalized!" << std::endl;
//	}

	//glfw_windows_handle = glfwCreateWindow(800, 600, "TEST", 0, 0);
    //glfwMakeContextCurrent(glfw_windows_handle);

    // Generic OS initialisation, creates a window and attaches OpenGL.
    Shell::Initialise("../Samples/tutorial/template/");
    Shell::OpenWindow("Template Tutorial", true);

    //! System interface
    ShellSystemInterface system_interface;
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

    Shell::LoadFonts("ui/");

    // Load and show the tutorial document.
    Rocket::Core::ElementDocument* document = context->LoadDocument("ui/tutorial.rml");
    if (document != NULL)
    {
        document->Show();
        document->RemoveReference();
    }


    Shell::EventLoop(GameLoop);

    // Shutdown Rocket.
    context->RemoveReference();
    Rocket::Core::Shutdown();

    Shell::CloseWindow();
    Shell::Shutdown();

	return 0;
}
*/
