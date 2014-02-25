#include "./utils/Common.h"
#include "./renderer/Context.h"
#include "./renderer/Renderer.h"
#include "./ui/Userinterface.h"
#include "./ui/UISystemInterface.h"

Rocket::Core::Context* context = 0;

void GameLoop()
{
    context->Update();
    context->Render();
}

int main(void)
{
    //renderer::Context* context = new renderer::Context( glm::ivec2(1024, 768) );
    //ui::UserInterface *ui = new ui::UserInterface(*context, RESOURCES_PATH);
    //context->createWindow("Test");
	renderer::Renderer* main_renderer;
    main_renderer = renderer::Renderer::instance();
    //main_renderer->setRenderContext(*context);
    /*
    ui->Initalize();

    UISystemInterface system_interface;
    Rocket::Core::SetSystemInterface(&system_interface);

    ShellRenderInterfaceOpenGL opengl_renderer;
    Rocket::Core::SetRenderInterface(&opengl_renderer);
    opengl_renderer.SetViewport(1024,768);

    ui->CreateContext("test", *context);
    ui->LoadUI("/ui/tutorial.rml");

    main_renderer->renderloop(*ui);
    */


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
        //renderer::Context* m_context = new renderer::Context(glm::ivec2(1024, 768));
        //m_context->InitalizeUsingUI();
        //m_context->createWindow("TEST");

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
            textBox->SetProperty("left", Rocket::Core::Property(140, Rocket::Core::Property::PX));
            textBox->SetProperty("top", Rocket::Core::Property(150, Rocket::Core::Property::PX));

            navBar->Show();
            navBar->SetProperty("left", Rocket::Core::Property(0, Rocket::Core::Property::PX));
            navBar->SetProperty("top", Rocket::Core::Property(0, Rocket::Core::Property::PX));
        }


        Shell::EventLoop(GameLoop);

        // Shutdown Rocket.
        context->RemoveReference();
        Rocket::Core::Shutdown();

        Shell::CloseWindow();
        Shell::Shutdown();

	return 0;
}
