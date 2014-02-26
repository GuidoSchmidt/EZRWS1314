#include "./utils/Common.h"
#include "./renderer/Context.h"
#include "./renderer/Renderer.h"
#include <Rocket/Core.h>
#include <Rocket/Debugger.h>
#include "./ui/UISystemInterface.h"
#include "./ui/shell/Shell.h"
#include "./ui/shell/Input.h"

#include "./ui/listeners/clicklistener.h"
#include <string>

Rocket::Core::Context* context = 0;

void GameLoop()
{
    context->Update();
    context->Render();
}

int main(void)
{
    renderer::Renderer* main_renderer;
    main_renderer = renderer::Renderer::instance();

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

        navBar->Show();
        navBar->SetProperty("left", Rocket::Core::Property(0, Rocket::Core::Property::PX));
        navBar->SetProperty("top", Rocket::Core::Property(0, Rocket::Core::Property::PX));
    }
    ClickListener::RegisterClickableContainer(navBar->GetElementById("vertex"));
    ClickListener::RegisterClickableContainer(navBar->GetElementById("geometry"));
    ClickListener::RegisterClickableContainer(navBar->GetElementById("tesselation"));
    ClickListener::RegisterClickableContainer(navBar->GetElementById("fragment"));


    //Shell::EventLoop(GameLoop);

    while(!glfwWindowShouldClose(glfwindow))
    {
        renderer::Renderer::instance()->renderloop();

        // LibRocket input handling
        int x = 0, y = 0;
        int key_modifier_state = -1;
        double xpos, ypos;
        glfwGetCursorPos(glfwindow, &xpos, &ypos);
        context->ProcessMouseMove(static_cast<int>(xpos), static_cast<int>(ypos), key_modifier_state);

        int button_index = 1;
        if(glfwGetMouseButton(glfwindow, GLFW_MOUSE_BUTTON_1) == GLFW_PRESS ||
           glfwGetMouseButton(glfwindow, GLFW_MOUSE_BUTTON_2) == GLFW_PRESS ||
           glfwGetMouseButton(glfwindow, GLFW_MOUSE_BUTTON_3) == GLFW_PRESS)
        {
			//std::cout << "Mouse Press" << std::endl;
            if(glfwGetMouseButton(glfwindow, GLFW_MOUSE_BUTTON_1))
                button_index = 0;
            if(glfwGetMouseButton(glfwindow, GLFW_MOUSE_BUTTON_2))
                button_index = 2;
            if(glfwGetMouseButton(glfwindow, GLFW_MOUSE_BUTTON_3))
                button_index = 1;

            context->ProcessMouseButtonDown(button_index, key_modifier_state);
			
			if(glfwGetMouseButton(glfwindow, GLFW_MOUSE_BUTTON_1) == GLFW_RELEASE
				   // glfwGetMouseButton(glfwindow, GLFW_MOUSE_BUTTON_2) == GLFW_RELEASE ||
				   // glfwGetMouseButton(glfwindow, GLFW_MOUSE_BUTTON_3) == GLFW_RELEASE
				   )
			{
				//std::cout << "Mouse Release" << std::endl;
				if(glfwGetMouseButton(glfwindow, GLFW_MOUSE_BUTTON_1))
					button_index = 0;
				if(glfwGetMouseButton(glfwindow, GLFW_MOUSE_BUTTON_2))
					button_index = 2;
				if(glfwGetMouseButton(glfwindow, GLFW_MOUSE_BUTTON_3))
					button_index = 1;

				context->ProcessMouseButtonUp(button_index, key_modifier_state);
			}
        }


        // Render loop
        GameLoop();

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
