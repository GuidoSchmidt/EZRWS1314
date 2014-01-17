
#pragma once
#ifndef H_FULLSCRNTRIANGLE
#define	H_FULLSCRNTRIANGLE

#include "../utils/Common.h"

namespace utils
{
	class FullscreenTriangle
	{
		private:
            GLuint m_VAO_handle;
            GLuint m_VBO_handle, m_IBO_handle, m_UVBO_handle;


		
		public:
            //! \brief Constructor: private because this class is singleton
            FullscreenTriangle();

            //! \brief Destructor
            ~FullscreenTriangle();

            //! \brief Creates the geometry buffers for drawing a screen filling triangle
            void createGeometry(void);

			//! \brief Returns the singleton instance
            //static FullscreenTriangle* instance(void);

			//! \brief Draws the screen filling triangle
            void draw(void);
	};
}

#endif //H_FULLSCRNTRIANGLE
