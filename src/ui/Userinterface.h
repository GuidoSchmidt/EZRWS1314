//! @file Userinterface.h
#ifndef USERINTERFACE_H
#define USERINTERFACE_H
#pragma once

#include <sys/types.h>
#include <sys/time.h>
#include <time.h>

#include "../renderer/Context.h"

#include <Rocket/Core/Types.h>
#include <Rocket/Core.h>
#include <Rocket/Debugger.h>

#include "shell/x11/InputX11.h"
#include "shell/ShellFileInterface.h"
#include "shell/Shell.h"

namespace ui
{
    class UserInterface
    {
        typedef void (*LoopFunction)();

        private:
            timeval m_start_time;
            ShellFileInterface* file_interface;
            Rocket::Core::Context* m_rocket_context;
            Rocket::Core::ElementDocument* m_rocket_document;

        public:
            UserInterface(renderer::Context& context);
            void Initalize(Rocket::Core::String&);
            void Draw();
            void SetLoop(LoopFunction function);
            void DisplayError(const char* fmt, ...);
            void Log(const char* fmt, ...);
            float GetElapsedTime();

            void CreateContext(const std::string name, renderer::Context& context);
            void LoadUI(const Rocket::Core::String& path);
    };

}

#endif // USERINTERFACE_H
