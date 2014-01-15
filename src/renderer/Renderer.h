//! @file Renderer.h
#pragma once
#ifndef H_RENDERER
#define H_RENDERER

#include "Context.h"
#include "ShaderProgram.h"
#include "FrameBufferObject.h"
#include "../scene/SceneNode.h"
#include "../scene/Geometry.h"
#include "../post/SlimFBO.h"
#include "../post/SlimShader.h"
#include "../post/SlimQuad.h"
#include "../post/GlowPass.h"
#include "../post/Sunlight/SeparatedBlurPass.h"
#include "../post/Sunlight/RadialGlowMaskPass.h"
#include "../post/Sunlight/RadialLuminancePass.h"
#include "../post/ToneMapping/CPUExtractionPass.h"
#include "../post/ToneMapping/MipMapExtractionPass.h"
#include "../post/FinalPass.h"
#include "../post/PhongPass.h"
#include "../utils/Importer.h"

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

			SlimQuad* fsq;
			SlimFBO* gBuffer;
			SlimFBO* lightingFBO;
			SlimFBO* sunlightFBO0;
			SlimFBO* sunlightFBO1;
			SlimFBO* sunlightFBO2;
			SlimFBO* sunlightFBO3;
			SlimFBO* sunlightFBO4;


			SlimFBO* downsampledExtractionFBO;


			double delta;
			double sunSpeed;
			glm::vec4 wsSunPos;
			glm::vec4 ssSunPos;
			float sunRadius;
			double sunAngle;
			SeparatedBlurPass* blurPass;
			RadialGlowMaskPass* maskPass;
			RadialLuminancePass* luminancePass;

			ISlimRenderPass* extractionPass;
			CPUExtractionPass* slowExtractionPass;
			MipMapExtractionPass* fastExtractionPass;
			FinalPass* finalPass;


			scene::Geometry* skyNode;
			scene::Geometry* shipBot;
			scene::Geometry* shipTop;
			scene::Geometry* shipSails;
			scene::Geometry* shipStuff;
            //PhongPass* phong1

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

            //! \brief Sets the needes shader stages and fbos
            void setupShaderStages(void);
				
		public:
			//! Returns the singleton instance
			static Renderer* instance(Context& context);

			//! \brief Sets the context to render to
            void setRenderContext(Context& context);

			//! \brief Calls the render loop
			void renderloop(void);

			void doTheSunlightEffect(void);

			void switchExtractionStrategy();
	};
}
#endif //H_RENDERER
