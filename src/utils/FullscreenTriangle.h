
#pragma once
#ifndef H_FULLSCRNTRIANGLE
#define	H_FULLSCRNTRIANGLE

#include "../utils/Common.h"

namespace utils
{
	class FullscreenTriangle
	{
		private:
			GLuint VAO_handle;
			GLuint VBO_handle, IBO_handle, NBO_handle, UVBO_handle;

			//! \brief Creates the geometry buffers for drawing a screen filling triangle
			void createGeometry(void);

			//! \brief Constructor: private because this class is singleton
			FullscreenTriangle();

			//! \brief Destructor
			~FullscreenTriangle();
		
		public:
			//! \brief Returns the singleton instance
			static FullscreenTriangle* instance(void);

			//! \brief Draws the screen filling triangle
			static void draw(void);
	};
}

#endif //H_FULLSCRNTRIANGLE
