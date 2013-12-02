#include "./utils/Common.h"
#include "./renderer/Context.h"
#include "./renderer/Renderer.h"

#include <Rocket/Core.h>
#include <Rocket/Debugger.h>
#include "./ui/ShellRenderInterfaceOpenGL.h"
#include "./ui/ShellSystemInterface.h"
#include "./ui/Shell.h"
#include "./ui/Input.h"

using namespace ezr;

/*
int main(void)
{
	//renderer::Context* context = new renderer::Context( glm::ivec2(1024, 768) );
	//renderer::Renderer* main_renderer;
	//main_renderer = renderer::Renderer::instance();
	//main_renderer->setRenderContext(*context);
	//main_renderer->renderloop();

	//! ----------- LibRocket -----------
	//! 1. Set up interfaces
	if (!Shell::Initialise("../Samples/tutorial/template/") ||
		!Shell::OpenWindow("Template Tutorial", true))
	{
		Shell::Shutdown();
		return -1;
	}

	ShellRenderInterfaceOpenGL opengl_renderer;
	Rocket::Core::SetRenderInterface(&opengl_renderer);
	opengl_renderer.SetViewport(1024, 768);

	ShellSystemInterface system_interface;
	Rocket::Core::SetSystemInterface(&system_interface);

	Rocket::Core::Initialise();

	// Create the main Rocket context and set it on the shell's input layer.
	Rocket::Core::Context* context = Rocket::Core::CreateContext("main", Rocket::Core::Vector2i(1024, 768));
	

	return 0;
}*/

Rocket::Core::Context* context = NULL;
Shell* shell = NULL;

void GameLoop()
{
	glClear(GL_COLOR_BUFFER_BIT);

	context->Update();
	context->Render();

	shell->FlipBuffers();
}

int main(int ROCKET_UNUSED(argc), char** ROCKET_UNUSED(argv))
{
	// Generic OS initialisation, creates a window and attaches OpenGL.
	//renderer::Context* render_context = new renderer::Context( glm::ivec2(1024, 768) );

	shell = new Shell();

	if (!Shell::Initialise("../Samples/tutorial/template/") ||
		!shell->OpenWindow("Template Tutorial", true))
	{
		Shell::Shutdown();
		return -1;
	}

	// Rocket initialisation.
	ShellRenderInterfaceOpenGL opengl_renderer;
	Rocket::Core::SetRenderInterface(&opengl_renderer);
	opengl_renderer.SetViewport(1024, 768);

	ShellSystemInterface system_interface;
	Rocket::Core::SetSystemInterface(&system_interface);

	Rocket::Core::Initialise();

	// Create the main Rocket context and set it on the shell's input layer.
	context = Rocket::Core::CreateContext("main", Rocket::Core::Vector2i(1024, 768));
	if (context == NULL)
	{
		Rocket::Core::Shutdown();
		Shell::Shutdown();
		return -1;
	}

	Rocket::Debugger::Initialise(context);
	Input::SetContext(context);

	Shell::LoadFonts("../EZRWS1314/resources/ui/fonts/");

	// Load and show the tutorial document.
	Rocket::Core::ElementDocument* document = context->LoadDocument("../EZRWS1314/resources/ui/test.rml");
	if (document != NULL)
	{
		document->Show();
		document->RemoveReference();
	}

	shell->EventLoop(GameLoop);

	// Shutdown Rocket.
	context->RemoveReference();
	Rocket::Core::Shutdown();

	Shell::CloseWindow();
	Shell::Shutdown();

	return 0;
}