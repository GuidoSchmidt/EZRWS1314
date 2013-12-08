//! @file Renderer.cpp
#include "Renderer.h"

namespace renderer {

    Renderer::Renderer(void)
    {
        //! Initialize fps variables
        m_framecount = 0;
        m_fps = 0;
        m_current_time = 0.0;
        m_previous_time = 0.0;

        init();
    }

    Renderer::~Renderer(void)
    {
    }

    Renderer* Renderer::instance(void)
    {
        static Renderer m_instance;
        return &m_instance;
    }

    void Renderer::setRenderContext(Context& context)
    {
        m_context = &context;
    }

    void Renderer::init()
    {
		std::cout << "Renderer.init() called" << std::endl;
        setupGL();
        setupShaderStages();
	
		//ToDo Modells laden
		//ToDo Texturen laden
		//Gui bauen


		//todo nicht lokal
		int WIDTH = 1280;
		int HEIGHT = 720;
		glm::vec2 nearFar = glm::vec2(0.1,60.0);
		
		//Setup dat slim fboooooos

        //gBuffer		= new SlimFBO(WIDTH,HEIGHT, 2, true);
        //lightingFBO = new SlimFBO(WIDTH,HEIGHT, 1, false);
        //glowFBO		= new SlimFBO(WIDTH,HEIGHT, 1, false);

		//now the render passses!
        //fsq = new SlimQuad();

        //phong1 = new PhongPass(fsq, nearFar,WIDTH,HEIGHT);//,mouseX,mouseY);
        //phong1->outputFBO = lightingFBO;
        //phong1->inputFBOs.push_back(gBuffer);

        //glowHalf = new GlowPass(1,fsq,WIDTH,HEIGHT);
        //glowHalf->outputFBO = glowFBO;
        //glowHalf->inputFBOs.push_back(lightingFBO);
    }
	
    void Renderer::setupGL(void)
    {
        //! OpenGL settings
        glClearColor(0.0, 0.0, 0.0, 1.0);
        glEnable(GL_DEPTH_TEST);
    }

    void Renderer::setupShaderStages()
    {
        m_shaderProgram_forward = new ShaderProgram(GLSL::VERTEX, "/shader/forward/forward.vs.glsl",
                                                    GLSL::FRAGMENT, "/shader/forward/forward.fs.glsl");
        m_shaderProgram_forward->Link();
    }

    void Renderer::renderloop()
    {
        while (m_context && m_context->isLive())
        {
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            //! Render calls here

			//ToDo
			//render shadow map

			//Render that sweetAsShit beach geometry
			//gBuffer->write();
			//drawBeach();

			//phong1->doExecute();
			
			//glowHalf->doExecute();
			
            //phong1->doExecute();

            //glowHalf->doExecute();

			//ideal pipeline würde so aussehen:
			//backBuffer << toneMappingPass << glowFBO << glowPass << lightingFBO << gBuffer << geometryPass

			//finalPassWithToneMapping->doExecute();

            m_context->swapBuffers();
            m_framecount++;
        }
    }
}
