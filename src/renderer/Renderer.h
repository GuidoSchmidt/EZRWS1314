//! @file Renderer.h
#pragma once
#ifndef H_RENDERER
#define H_RENDERER

#define GLFW_DLL
#define GLFW_CDECL
#include "ShaderProgram.h"
#include "FrameBufferObject.h"
#include "Context.h"

namespace tone {
	namespace renderer {
		//! @class Renderer
		class Renderer
		{
			private:
				//! Time and frame variables for fps calculation
				unsigned int m_framecount;
				int m_fps;
				double m_current_time, m_previous_time;

				/*! Constructor and destrcutor are private because
				 *	Renderer is implemented as signleton class
				 */
				Renderer(void);
				~Renderer(void);
				void init(void);
				
				//! Context, which holds a window to render to
				Context* m_context;

				//! Shader programs used
				ShaderProgram* m_shaderProgram_forward;

			public:
				static Renderer& instance(void);
				void Renderer::setRenderContext(Context& context);
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

		static Renderer& renderer(void) { return Renderer::instance(); }
	}
}
#endif //H_RENDERER
