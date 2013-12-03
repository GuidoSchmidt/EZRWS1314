//! @file Renderer.h
#pragma once
#ifndef H_RENDERER
#define H_RENDERER

//#include <Rocket/Core/Context.h>
//#include "../ui/Shell.h"
#include "Context.h"
#include "FrameBufferObject.h"
#include "ShaderProgram.h"

namespace renderer {		
	//! @class Renderer
	class Renderer
	{
		private:
			unsigned int m_framecount;
			int m_fps;
			double m_current_time, m_previous_time;

			Context* m_context;
			ShaderProgram* m_shaderProgram_forward;

			//Rocket::Core::Context* context;
			//Shell* shell;

			//! \brief Constructor (singleton)
			Renderer(void);

			//! \brief Destructor (singleton)
			~Renderer(void);

			//! \brief Initializes the renderer
			void init(void);

			//! \brief Sets needed OpenGL states
			void setupGL(void);
				
		public:
			//! Returns the singleton instance
			static Renderer* instance(void);

			//! \brief Sets the context to render to
            void setRenderContext(Context& context);

			//! \brief Calls the render loop
			void renderloop(void);

			//! Input handling
			/*
			//! Mouse callback function
			void mouseCallback_implementation(int button, int action);
			static void mouseCallback(GLFWwindow* window, int button, int action, int mods)
			{
				static Renderer* renderer = &Renderer::instance();
				renderer->mouseCallback_implementation(button, action);
			}
			//! Keyboard callback function
			void keyboardCallback_implementation(int key, int action);
			static void keyboardCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
			{
				static Renderer* renderer = &Renderer::instance();
				renderer->keyboardCallback_implementation(key, action);
			}
			void cursorCallback_implementation(double mousex, double mousey);
			static void cursorCallback(GLFWwindow* window, double mousex, double mousey)
			{
				static Renderer* renderer = &Renderer::instance();
				renderer->cursorCallback_implementation(mousex, mousey);
			}
			*/
	};
}
#endif //H_RENDERER
