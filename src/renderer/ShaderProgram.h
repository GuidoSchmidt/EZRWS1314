//! @file ShaderProgram.h
/*!
 * @date	30.11.2012
 * @author	Guido Schmidt
 */

#pragma once
#ifndef H_SHADERPROGRAM
#define H_SHADERPROGRAM

#include "../utils/Common.h"
#include <vector>
#include <fstream>
#include <iostream>
#include <algorithm>
#include <cctype>

namespace renderer {
	//! Shader program helper function
	namespace GLSL{
		enum GLSLShaderType{
			VERTEX = 0,
            FRAGMENT = 1,
			GEOMETRY = 2,
			TESS_CONTROL = 3,
			TESS_EVALUATION = 4
		};

		//! \brief Reads the source code for a shader
		std::string ReadShaderSource(const std::string filename);

		//! \brief Prints the shader info log
		void PrintShaderInfoLog(GLuint shader);

		//! \brief Prints the shader programs info log
		void PrintShaderProgramInfoLog(GLuint shaderProgram);
	}

	//! @class ShaderProgram
	class ShaderProgram
	{
		private:
			GLint m_shaderProgram_ID;
            std::map<GLSL::GLSLShaderType, GLint> m_shader_IDs;
            //std::vector<std::string> m_shader_sources;
            std::map<GLSL::GLSLShaderType, std::string> m_shader_sources;
			bool m_islinked;
			bool m_activeUniformsWritten;
			bool m_activeAttributesWritten;
			GLenum m_errorCheckValue;
			std::string m_log;

			//! \brief Returns the location of a given uniform variable
            int getUniformLocation(std::string name);

			//! \brief Checks if a file exists
            bool checkFileExist(const std::string &filename);

		public:
			//! \brief Constructor
			ShaderProgram();

			//! \brief Constructor
			ShaderProgram(GLSL::GLSLShaderType shaderType0, std::string filename0, GLSL::GLSLShaderType shaderType1, std::string filename1);
			
			//! \brief Destructor
			~ShaderProgram();

			//! \brief Adds a shader to the program
            void addShader(GLSL::GLSLShaderType shaderType, std::string filename);

			//! \brief Links the shader program
            void link(void);

			//! \brief Checks the link status of the shader program
            bool isLinked(void);

			//! \brief Activates the shader program for use
            void use(void);

			//! \brief Deactivates the shader program from use
            void unuse(void);

			//! \brief Reloads all shaders
            void reloadAllShaders(void);

			//! \brief Reloads a specific shader
            void ReloadShader(GLSL::GLSLShaderType shaderType);

			//! \brief Returns the program log
            std::string log(void);

			//! \brief Returns the shader program handle
            GLint getHandle(void);

            //! \brief Returns a uniform handle by its name used in the prorgam
            GLuint getUniform(std::string uniform_name);

			//! \brief Binds an attribute location
            void bindAttributeLocation(GLuint location, std::string name);

			//! \brief Binds a fragment data location 
            void bindFragDataLocation(GLuint location, std::string* name);

			//! \brief Sets a uniform variable for a 2D vector (vec2)
            void setUniform(GLuint uniform_location, const glm::vec2 &vec);

			//! \brief Sets a uniform variable for a 3D vector (vec3)
            void setUniform(GLuint uniform_location, const glm::vec3 &vec);

			//! \brief Sets a uniform variable for a 4D vector (vec4)
            void setUniform(GLuint uniform_location, const glm::vec4 &vec);

			//! \brief Sets a uniform variable for a 4x4 matrix (mat4)
            void setUniform(GLuint uniform_location, const glm::mat4 &mat);

			//! \brief Sets a uniform variable for a float value
            void setUniform(GLuint uniform_location, float val);

			//! \brief Sets a uniform variable for an integer value
            void setUniform(GLuint uniform_location, int val);

			//! \brief Sets a uniform variable for an array of 3D-floats
            void setUniformArray3f(GLuint uniform_location, int count, GLfloat* value_ptr);

			//! \brief Sets a uniform variable for an array of 4x4 matrices
            void setUniformArrayMatrix4f(GLuint uniform_location, int count, GLfloat* value_ptr);

			//! \brief Sets a uniform variable for a texture sampler
            void setUniformSampler(GLuint uniform_location, GLuint texture, int textureUnit);

			//! \brief Sets a uniform variable for a cube map texture
            void setUniformCubemap(GLuint uniform_location, GLuint texture, int textureUnit);

			//! \brief Prints all active uniforms
            void printActiveUniforms(void);

			//! \brief Prints all active attributes
            void printActiveAttributes(void);

            //! \brief Returns the code of a shader
			std::string getShaderCodeOf(GLSL::GLSLShaderType shaderType, unsigned int &lineCount);
	};
}
#endif //H_SHADERPROGRAM
