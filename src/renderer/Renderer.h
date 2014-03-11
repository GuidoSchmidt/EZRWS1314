//! @file Renderer.h
#pragma once
#ifndef H_RENDERER
#define H_RENDERER

#include "Context.h"
#include "ShaderProgram.h"
#include "FrameBufferObject.h"
#include "../scene/SceneNode.h"
#include "../scene/SceneManager.h"
#include "../scene/Geometry.h"
#include "../scene/Sun.h"
#include "../post/SlimFBO.h"
#include "../post/SlimShader.h"
#include "../post/SlimQuad.h"
#include "../post/GlowPass.h"
#include "../post/Sunlight/SeparatedBlurPass.h"
#include "../post/Sunlight/RadialGlowMaskPass.h"
#include "../post/Sunlight/RadialLuminancePass.h"
#include "../post/ToneMapping/CPUExtractionPass.h"
#include "../post/CompositingPass.h"
#include "../post/ToneMapping/MipMapExtractionPass.h"
#include "../post/FinalPass.h"
#include "../post/PhongPass.h"
#include "../utils/Importer.h"
#include "../utils/FullscreenTriangle.h"


namespace renderer {		
	//! @class Renderer
	class Renderer
	{
		private:
			unsigned int m_framecount;
			int m_fps;
			double m_current_time, m_previous_time;

			Context* m_context;
            //! Shader programs
            ShaderProgram* m_shaderProgram_forward;
			ShaderProgram* m_shaderProgram_sky;
            ShaderProgram* m_shaderProgram_compositing;
            //! Framebuffer objects
            FrameBufferObject* m_fbo;

            //! Scene
            utils::FullscreenTriangle* m_fullscreen_triangle;
			scene::Camera* m_scene_camera;
			std::vector<scene::Geometry*> m_renderqueue;

			SlimQuad* fsq;
			SlimFBO* gBuffer;
			SlimFBO* lightingFBO;
			SlimFBO* sunlightFBO0;
			SlimFBO* sunlightFBO1;
			SlimFBO* sunlightFBO2;
			SlimFBO* sunlightFBO3;
			SlimFBO* sunlightFBO4;

			SlimFBO* compositingFBO;

			SlimFBO* downsampledExtractionFBO;

			SeparatedBlurPass* blurPass;
			RadialGlowMaskPass* maskPass;
			RadialLuminancePass* luminancePass;
			CompositingPass* compositingPass;

			ISlimRenderPass* extractionPass;
			CPUExtractionPass* slowExtractionPass;
			MipMapExtractionPass* fastExtractionPass;
			FinalPass* finalPass;


			glm::mat4 skyScale;
			scene::Sun* sun;
			scene::Geometry* skyNode;


			//! \brief Constructor (singleton)
			Renderer(void);

			//! \brief Destructor (singleton)
			~Renderer(void);



			//! \brief Sets needed OpenGL states
			void setupGL(void);

            //! \brief Sets the needes shader stages and fbos
			void setupShaderStages(void);
				
		public:
			//! Returns the singleton instance
            static Renderer* instance();

			//! \brief Initializes the renderer
			void init(GLFWwindow* window);

			//! \brief Sets the context to render to
			void setRenderContext(Context& context);

			//! \brief Setup the render loop
			void Renderer::setupRenderer(GLFWwindow* window);

			//! \brief Calls the render loop
            void renderloop(GLFWwindow *window);

			void doTheSunlightEffect(void);

			void switchExtractionStrategy();
            //! \brief Returns the source code of a shader of a shader program
            std::string getShaderSourceOf(GLSL::GLSLShaderType shaderType);
	};
}
#endif //H_RENDERER
