//! @file Context.h
/*!
*	@date 	03.12.2013
*	@author	Guido Schmidt
*/
#pragma once
#ifndef CONTEXT_H
#define CONTEXT_H

#include "../utils/Common.h"

namespace renderer {
	class Context
	{
		private:
			GLFWwindow* m_window;
			glm::ivec2 m_size;
			std::string m_title;

		public:
			//! \brief Constructor
			Context(const glm::ivec2& size);
				
			//!\brief Destructor
			~Context();

			//! \brief Swaps the back and the front buffer
			void swapBuffers(void);

			//! \brief Checks if the context is used at the moment
			bool isLive(void);

			//! \brief Returns the size of the context
			glm::ivec2 getSize(void);

			//! \brief Returns the window
			GLFWwindow* getWindow(void);

			//! \brief Creates a window
			void createWindow(std::string title);
	};
}

#endif //CONTEXT_H
