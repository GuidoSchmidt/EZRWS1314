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
			std::vector<GLint> m_shader_IDs;
			std::vector<std::string> m_shader_sources;
			bool m_islinked;
			bool m_activeUniformsWritten;
			bool m_activeAttributesWritten;
			GLenum m_errorCheckValue;
			std::string m_log;

			//! \brief Returns the location of a given uniform variable
			int GetUniformLocation(std::string name);

			//! \brief Checks if a file exists
			bool CheckFileExist(const std::string &filename);

		public:
			//! \brief Constructor
			ShaderProgram();

			//! \brief Constructor
			ShaderProgram(GLSL::GLSLShaderType shaderType0, std::string filename0, GLSL::GLSLShaderType shaderType1, std::string filename1);
			
			//! \brief Destructor
			~ShaderProgram();

			//! \brief Adds a shader to the program
			void AddShader(GLSL::GLSLShaderType shaderType, std::string filename);

			//! \brief Links the shader program
			void Link(void);

			//! \brief Checks the link status of the shader program
			bool IsLinked(void);

			//! \brief Activates the shader program for use
			void Use(void);

			//! \brief Deactivates the shader program from use
			void Unuse(void);

			//! \brief Reloads all shaders
			void ReloadAllShaders(void);

			//! \brief Reloads a specific shader
			void ReloadShader(int i);

			//! \brief Returns the program log
			std::string Log(void);

			//! \brief Returns the shader program handle
			GLint GetHandle(void);

			//! \brief Binds an attribute location
			void BindAttributeLocation(GLuint location, std::string name);

			//! \brief Binds a fragment data location 
			void BindFragDataLocation(GLuint location, std::string* name);

			//! \brief Sets a uniform variable for a 2D vector (vec2)
			void SetUniform(const std::string name, const glm::vec2 &vec);

			//! \brief Sets a uniform variable for a 3D vector (vec3)
			void SetUniform(const std::string name, const glm::vec3 &vec);

			//! \brief Sets a uniform variable for a 4D vector (vec4)
			void SetUniform(const std::string name, const glm::vec4 &vec);

			//! \brief Sets a uniform variable for a 4x4 matrix (mat4)
			void SetUniform(const std::string name, const glm::mat4 &mat);

			//! \brief Sets a uniform variable for a float value
			void SetUniform(const std::string name, float val);

			//! \brief Sets a uniform variable for an integer value
			void SetUniform(const std::string name, int val);

			//! \brief Sets a uniform variable for an array of 3D-floats
			void SetUniformArray3f(const std::string name, int count, GLfloat* value_ptr);

			//! \brief Sets a uniform variable for an array of 4x4 matrices
			void SetUniformArrayMatrix4f(const std::string name, int count, GLfloat* value_ptr);

			//! \brief Sets a uniform variable for a texture sampler
			void SetUniformSampler(const std::string name, GLuint texture, int textureUnit);

			//! \brief Sets a uniform variable for a texture multisample sampler
			void SetUniformSampler2DMS(const std::string name, GLuint texture, int textureUnit);

			//! \brief Sets a uniform variable for a cube map texture
			void SetUniformCubemap(const std::string name, GLuint texture, int textureUnit);

			//! \brief Prints all active uniforms
			void PrintActiveUniforms(void);

			//! \brief Prints all active attributes
			void PrintActiveAttributes(void);
	};
}
#endif //H_SHADERPROGRAM