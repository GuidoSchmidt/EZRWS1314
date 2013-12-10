#include "./utils/Common.h"
#include "./renderer/Context.h"
#include "./renderer/Renderer.h"

#include <Rocket/Core.h>
#include <Rocket/Debugger.h>
#include "./ui/ShellRenderInterfaceOpenGL.h"
#include "./ui/Shell.h"

Rocket::Core::Context* context = NULL;

void GameLoop()
{
    glClear(GL_COLOR_BUFFER_BIT);
    context->Update();
    context->Render();

    Shell::FlipBuffers();
}

int main(void)
{
    /*
	renderer::Context* context = new renderer::Context( glm::ivec2(1024, 768) );
	renderer::Renderer* main_renderer;
    main_renderer = renderer::Renderer::instance();
    main_renderer->setRenderContext(*context);
    main_renderer->renderloop();
    */

//! ***** LibRocket Test *****

    if (!Shell::Initialise("../Samples/tutorial/template/") ||
        !Shell::OpenWindow("Template Tutorial", true))
    {
        Shell::Shutdown();
        return -1;
    }
    //! Rocket: Set render interface
    ShellRenderInterfaceOpenGL opengl_renderer;
    Rocket::Core::SetRenderInterface(&opengl_renderer);
    //! Rocket: Set system interface
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

    //! Rocket: Initialize debugger
    //Rocket::Debugger::Initialise(context);
    //Input::SetContext(context);

    //! Rocket: Load Fonts
    Shell::LoadFonts(RESOURCES_PATH "/ui/fonts/Roboto/");

    // Load and show the demo document.
    Rocket::Core::ElementDocument* document = context->LoadDocument(RESOURCES_PATH "/ui/rocket/tutorial.rml");
    if (document != NULL)
    {
        std::cout << "la" << std::endl;
        document->Show();
    }

    Shell::EventLoop(GameLoop);

    // Shutdown Rocket.
    document->RemoveReference();
    context->RemoveReference();
    Rocket::Core::Shutdown();

    Shell::CloseWindow();
    Shell::Shutdown();

	return 0;
}
