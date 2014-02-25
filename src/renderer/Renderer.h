//! @file Renderer.h
#pragma once
#ifndef H_RENDERER
#define H_RENDERER

//#include <Rocket/Core/Context.h>
//#include "../ui/Shell.h"
#include "Context.h"
#include "FrameBufferObject.h"
#include "ShaderProgram.h"
#include "../ui/Userinterface.h"

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
	};
}
#endif //H_RENDERER
