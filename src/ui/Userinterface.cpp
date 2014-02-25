//! @file Userinterface.cpp
#include "Userinterface.h"

namespace ui
{
    UserInterface::UserInterface(renderer::Context& context)
    {

        m_rocket_context = 0;
    }

    void UserInterface::Initalize(Rocket::Core::String& filepath)
    {
        gettimeofday(&m_start_time, 0);
        InputX11::Initialise();

        glfwInit();

        file_interface = new ShellFileInterface(filepath);
        Rocket::Core::SetFileInterface(file_interface);
    }

    void UserInterface::SetLoop(LoopFunction function)
    {
        while(true)
        {
            function();
            glfwPollEvents();
        }
    }

    void UserInterface::DisplayError(const char* fmt, ...)
    {
        const int buffer_size = 1024;
        char buffer[buffer_size];
        va_list argument_list;

        // Print the message to the buffer.
        va_start(argument_list, fmt);
        int len = vsnprintf(buffer, buffer_size - 2, fmt, argument_list);
        if ( len < 0 || len > buffer_size - 2 )
        {
            len = buffer_size - 2;
        }
        buffer[len] = '\n';
        buffer[len + 1] = '\0';
        va_end(argument_list);

        printf("%s", buffer);
    }

    void UserInterface::Log(const char* fmt, ...)
    {
        const int buffer_size = 1024;
        char buffer[buffer_size];
        va_list argument_list;

        // Print the message to the buffer.
        va_start(argument_list, fmt);
        int len = vsnprintf(buffer, buffer_size - 2, fmt, argument_list);
        if ( len < 0 || len > buffer_size - 2 )
        {
            len = buffer_size - 2;
        }
        buffer[len] = '\n';
        buffer[len + 1] = '\0';
        va_end(argument_list);

        printf("%s", buffer);
    }

    float UserInterface::GetElapsedTime()
    {
        struct timeval now;

        gettimeofday(&now, NULL);

        double sec = now.tv_sec - m_start_time.tv_sec;
        double usec = now.tv_usec - m_start_time.tv_usec;
        double result = sec + (usec / 1000000.0);

        return (float)result;
    }

    void UserInterface::CreateContext(const std::string name, renderer::Context& context)
    {
        Rocket::Core::Initialise();

        m_rocket_context = Rocket::Core::CreateContext("name", Rocket::Core::Vector2i(context.getSize().x, context.getSize().y));
        if (m_rocket_context == 0)
        {
            Rocket::Core::Shutdown();
            std::cerr << "ERROR: User interface context creation failed!" << std::endl;
        }

        Rocket::Debugger::Initialise(m_rocket_context);
        Input::SetContext(m_rocket_context);

        Shell::LoadFonts(RESOURCES_PATH "/ui/fonts/");
    }

    void UserInterface::LoadUI(const Rocket::Core::String& path)
    {
        m_rocket_document = m_rocket_context->LoadDocument(path);
        if (m_rocket_document != NULL)
        {
            m_rocket_document->Show();
            m_rocket_document->RemoveReference();
        }
    }

    void UserInterface::Draw()
    {
        glClear(GL_COLOR_BUFFER_BIT);

        m_rocket_context->Update();
        m_rocket_context->Render();

    }
}
