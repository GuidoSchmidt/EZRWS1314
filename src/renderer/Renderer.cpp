//! @file Renderer.cpp
#include "Renderer.h"

double scroll = 60.0;

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

    Renderer* Renderer::instance(Context& context)
    {
        static Renderer m_instance;
        m_instance.setRenderContext(context);
        return &m_instance;
    }

    void Renderer::setRenderContext(Context& context)
    {
        m_context = &context;
        init();
    }

    void Renderer::init()
    {
		std::cout << "Renderer.init() called" << std::endl;
        setupGL();
        setupShaderStages();
    
        //! \todo Loads models via utils::Importer

        //utils::Importer::instance()->importFile(RESOURCES_PATH "/scenes/dae/simple_cube.dae");
		//utils::Importer::instance()->importFile(RESOURCES_PATH "/scenes/obj/baum_test.obj");
		utils::Importer::instance()->importFile(RESOURCES_PATH "/scenes/obj/sky.obj");
		skyNode = utils::Importer::instance()->getGeometryNode(0);
		utils::Importer::instance()->deleteGeometryNode(0);
		scene::SceneManager::instance()->deleteGeometryNode(0);

		//utils::Importer::instance()->importFile(RESOURCES_PATH "/scenes/obj/ship.obj");
		

		//utils::Importer::instance()->importFile(RESOURCES_PATH "/scenes/dae/house5.dae");
		utils::Importer::instance()->importFile(RESOURCES_PATH "/scenes/obj/house6.obj");

		//utils::Importer::instance()->importFile(RESOURCES_PATH "/scenes/dae/baum_test.dae");
        //utils::Importer::instance()->importFile(RESOURCES_PATH "/scenes/dae/simple_cube.dae");

        //utils::Importer::instance()->importFile(RESOURCES_PATH "/scenes/dae/head.dae");
        m_renderqueue = scene::SceneManager::instance()->generateRenderQueue();

        //! \todo Create user interface

        //! \todo Use Context::getSize().x and .y!
        int WIDTH = m_context->getSize().x;
        int HEIGHT = m_context->getSize().y;
        //glm::vec2 nearFar = glm::vec2(0.1,.0);
        
        //Setup dat slim fboooooos

        gBuffer		 = new SlimFBO(WIDTH,HEIGHT, 2, true);
		sunlightFBO0 = new SlimFBO(WIDTH / 4, HEIGHT / 4, 1, false);
		sunlightFBO1 = new SlimFBO(WIDTH / 4, HEIGHT / 4, 1, false);
		sunlightFBO2 = new SlimFBO(WIDTH / 4, HEIGHT / 4, 1, false);
		sunlightFBO3 = new SlimFBO(WIDTH / 4, HEIGHT / 4, 1, false);
		sunlightFBO4 = new SlimFBO(WIDTH / 4, HEIGHT / 4, 1, false);

		compositingFBO = new SlimFBO(WIDTH, HEIGHT, 1, false);

		downsampledExtractionFBO = new SlimFBO(WIDTH / 4, HEIGHT / 4, 1, false);

		//now the render passses!
        fsq = new SlimQuad();

		//now the render passses!
		blurPass = new SeparatedBlurPass(fsq, WIDTH/4, HEIGHT/4);
		blurPass->inputFBOs.push_back(gBuffer);

		maskPass = new RadialGlowMaskPass(fsq, WIDTH/4, HEIGHT/4);
		maskPass->outputFBO = sunlightFBO3;
		maskPass->inputFBOs.push_back(sunlightFBO2);

		luminancePass =  new RadialLuminancePass(fsq, WIDTH/4, HEIGHT/4);
		luminancePass->outputFBO = sunlightFBO4;
		luminancePass->inputFBOs.push_back(sunlightFBO3);

		compositingPass = new CompositingPass(fsq, WIDTH, HEIGHT);
		compositingPass->outputFBO = compositingFBO;
		compositingPass->inputFBOs.push_back(gBuffer);
		compositingPass->inputFBOs.push_back(sunlightFBO4);

		slowExtractionPass = new CPUExtractionPass(fsq, WIDTH, HEIGHT);
		slowExtractionPass->inputFBOs.push_back(compositingFBO);
		slowExtractionPass->inputFBOs.push_back(downsampledExtractionFBO);

		fastExtractionPass = new MipMapExtractionPass(fsq, WIDTH, HEIGHT);
		fastExtractionPass->inputFBOs.push_back(compositingFBO);

		extractionPass = fastExtractionPass;

		
		finalPass = new FinalPass(fsq, WIDTH, HEIGHT);
		finalPass->outputFBO = lightingFBO;
		finalPass->inputFBOs.push_back(gBuffer);
		finalPass->inputFBOs.push_back(sunlightFBO3);
		finalPass->inputFBOs.push_back(sunlightFBO4);
		finalPass->inputFBOs.push_back(compositingFBO);

		//setup sun
		skyScale = glm::mat4(10);
		skyScale[3][3]=1;
		scene::Transform trans = scene::Transform(glm::vec3(0),glm::toQuat(glm::mat4(1)),glm::vec3(1));
		GLint sunTex = scene::SceneManager::instance()->loadTexture(RESOURCES_PATH "/textures/common/sun.png",true);
		sun = new scene::Sun(1337,"zunLigt", trans, glm::vec3(1),1,990,sunTex);
		sun->setHour(12);
		sun->setMinute(0);
        renderloop();

    }
    
    void Renderer::setupGL(void)
    {
        //! OpenGL settings
        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

        glEnable(GL_DEPTH_TEST);
		glEnable(GL_TEXTURE_2D);
    }

    void Renderer::setupShaderStages()
    {
        //! Simple forward rendering
        m_shaderProgram_forward = new ShaderProgram(GLSL::VERTEX, RESOURCES_PATH "/shader/forward/forward.vs.glsl",
		                                            GLSL::FRAGMENT, RESOURCES_PATH "/shader/forward/forward.fs.glsl");
        m_shaderProgram_forward->link();

		m_shaderProgram_sky = new ShaderProgram(GLSL::VERTEX, RESOURCES_PATH "/shader/forward/forward.vs.glsl",
		                                            GLSL::FRAGMENT, RESOURCES_PATH "/shader/forward/sky.fs.glsl");
        m_shaderProgram_sky->link();
        //m_fbo = new FrameBufferObject(m_context->getSize().x, m_context->getSize().y);
        //m_fbo->addColorAttachment(0);
        //m_fbo->addDepthAttachment_Texture(1);

        //! Compositing rendering on fullscreen quad
        //m_shaderProgram_compositing = new ShaderProgram(GLSL::VERTEX, RESOURCES_PATH "/shader/compositing/fullscreen.vs.glsl",
        //                                                GLSL::FRAGMENT, RESOURCES_PATH "/shader/compositing/compositing.fs.glsl");
        //m_shaderProgram_compositing->link();
        //m_fullscreen_triangle = new utils::FullscreenTriangle();
    }

//! ---- Input Handling --------------------------------------------------------
/*!
*  \todo refractor to class
*/
    void ScrollCallback(GLFWwindow * window, double xoffset, double yoffset)
    {
        scroll += yoffset;

    }

    void KeyboardCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
    {
		/*if (key== GLFW_KEY_E)
			Renderer::instance(*/
    }

    void Renderer::renderloop()
    {
		glm::vec4 wsSunPos = glm::vec4(0,10,2000,1);

		GLuint night_tex = scene::SceneManager::instance()->loadTexture(RESOURCES_PATH "/textures/common/night.jpg",true);
		GLuint day_tex = scene::SceneManager::instance()->loadTexture(RESOURCES_PATH "/textures/common/day.jpg",true);
		//GLuint ldr_diffuse_cube = scene::SceneManager::instance()->loadCubeMap(RESOURCES_PATH "/textures/ldr-cross/beach_small_diffuse_cross", false);
		//GLuint ldr_reflective_cube = scene::SceneManager::instance()->loadCubeMap(RESOURCES_PATH "/textures/ldr-cross/beach_small_reflective_cross", false);


        //! Render calls here
        m_scene_camera = new scene::Camera(0,"scene_camera",
                           glm::vec3(30.0f, 30.0f, 30.0f),
                           glm::vec3(0.0f, 0.0f, 0.0f),
                           glm::vec3(0.0f, 1.0f, 0.0f),
                           m_context->getSize());
		m_scene_camera->SetFarPlane(3000);
		m_scene_camera->SetNearPlane(0.1);

        //! Uniform setup
        //! Forward shading
        GLuint forward_uniform_loc_view             = m_shaderProgram_forward->getUniform("view");
        GLuint forward_uniform_loc_projection       = m_shaderProgram_forward->getUniform("projection");
        GLuint forward_uniform_loc_model            = m_shaderProgram_forward->getUniform("model");
        GLuint forward_uniform_loc_diffuse_color    = m_shaderProgram_forward->getUniform("diffuse_color");
        GLuint forward_uniform_loc_diffuse_tex      = m_shaderProgram_forward->getUniform("diffuse_map");
        GLuint forward_uniform_loc_specular_color   = m_shaderProgram_forward->getUniform("specular_color");
        GLuint forward_uniform_loc_specular_tex     = m_shaderProgram_forward->getUniform("specular_map");
        GLuint forward_uniform_loc_shininess        = m_shaderProgram_forward->getUniform("shininess");
        GLuint forward_uniform_loc_normal_tex       = m_shaderProgram_forward->getUniform("normal_map");
        GLuint forward_uniform_loc_light_position   = m_shaderProgram_forward->getUniform("light_position");
		GLuint forward_uniform_loc_light_color      = m_shaderProgram_forward->getUniform("light_color");
        GLuint forward_uniform_loc_mouse            = m_shaderProgram_forward->getUniform("mouse");

		//! Sky shading
        GLuint sky_uniform_loc_model		    = m_shaderProgram_sky->getUniform("model");
        GLuint sky_uniform_loc_view             = m_shaderProgram_sky->getUniform("view");
        GLuint sky_uniform_loc_projection       = m_shaderProgram_sky->getUniform("projection");
        GLuint sky_uniform_loc_day_tex          = m_shaderProgram_sky->getUniform("day_tex");
        GLuint sky_uniform_loc_night_tex        = m_shaderProgram_sky->getUniform("night_tex");
        GLuint sky_uniform_loc_blend			= m_shaderProgram_sky->getUniform("blend");
        GLuint sky_uniform_loc_color            = m_shaderProgram_sky->getUniform("color");


        //! Compositing
        //GLuint compositing_uniform_loc_shadowMap    = m_shaderProgram_compositing->getUniform("shadowMap");
        //GLuint compositing_uniform_loc_lightedMap   = m_shaderProgram_compositing->getUniform("lightedMap");

        glm::vec3 camera_position = glm::vec3(1.0f);
        float camera_speed = 1;

        //scene::SceneManager::instance()->getLight(0)->setupShadowMapping(glm::vec2(512));

		double mouse_x, mouse_y, old_x, old_y;
		old_x = old_y = mouse_x = mouse_y = 0;
        while (m_context && m_context->isLive() && !glfwGetKey(m_context->getWindow(), GLFW_KEY_ESCAPE) )
        {
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            
            //! simple camera movement
            
			
            float  mouse_correct_x, mouse_correct_y;
			old_x=mouse_x;
			old_y=mouse_y;
            glfwGetCursorPos(m_context->getWindow(), &mouse_x, &mouse_y);
  /*          mouse_correct_x = ((mouse_x / m_context->getSize().x) * 2.0f) - 1.0f;
            mouse_correct_y = ((mouse_y / m_context->getSize().y) * 2.0f) - 1.0f;*/
			mouse_correct_x = (((mouse_x-old_x) / m_context->getSize().x));// * 2.0f) - 1.0f;
            mouse_correct_y = (((mouse_y-old_y) / m_context->getSize().y));// * 2.0f) - 1.0f;
            if (glfwGetMouseButton(m_context->getWindow(), GLFW_MOUSE_BUTTON_2))
            {
                  m_scene_camera->Rotate(mouse_correct_x * camera_speed * 100,
                                         mouse_correct_y * camera_speed * 100);
            }
			
            if (glfwGetKey(m_context->getWindow(), GLFW_KEY_W))
            {
                  m_scene_camera->MoveZ( camera_speed);
            }
            if (glfwGetKey(m_context->getWindow(), GLFW_KEY_S))
            {
                  m_scene_camera->MoveZ(-camera_speed);
            }
            if (glfwGetKey(m_context->getWindow(), GLFW_KEY_D))
            {
                  m_scene_camera->MoveX( camera_speed);
            }
            if (glfwGetKey(m_context->getWindow(), GLFW_KEY_A))
            {
                  m_scene_camera->MoveX(-camera_speed);
            }
			if (glfwGetKey(m_context->getWindow(), GLFW_KEY_UP))
			{
				sun->setHour(sun->getHour()+1);
			}
			if (glfwGetKey(m_context->getWindow(), GLFW_KEY_DOWN))
			{
				sun->setHour(sun->getHour()-1);
			}
			if (glfwGetKey(m_context->getWindow(), GLFW_KEY_LEFT))
			{
				
				sun->setMinute(sun->getMinute()-1);
				if (sun->getMinute() == 0)
					sun->setHour(sun->getHour()-1);
			}
			if (glfwGetKey(m_context->getWindow(), GLFW_KEY_RIGHT))
			{
				if (sun->getMinute() == 59)
					sun->setHour(sun->getHour()+1);
				sun->setMinute(sun->getMinute()+1);
			}
			if (glfwGetKey(m_context->getWindow(), GLFW_KEY_KP_2))
			{
				sun->setHour(0);
				sun->setMinute(0);

			}
			if (glfwGetKey(m_context->getWindow(), GLFW_KEY_KP_1))
			{
				sun->setHour(3);
				sun->setMinute(0);
			}
			if (glfwGetKey(m_context->getWindow(), GLFW_KEY_KP_4))
			{
				sun->setHour(6);
				sun->setMinute(0);
			}
			if (glfwGetKey(m_context->getWindow(), GLFW_KEY_KP_7))
			{
				sun->setHour(9);
				sun->setMinute(0);
			}
			if (glfwGetKey(m_context->getWindow(), GLFW_KEY_KP_8))
			{
				sun->setHour(12);
				sun->setMinute(0);
			}
			if (glfwGetKey(m_context->getWindow(), GLFW_KEY_KP_9))
			{
				sun->setHour(15);
				sun->setMinute(0);
			}
			if (glfwGetKey(m_context->getWindow(), GLFW_KEY_KP_6))
			{
				sun->setHour(18);
				sun->setMinute(0);
			}
			if (glfwGetKey(m_context->getWindow(), GLFW_KEY_KP_3))
			{
				sun->setHour(21);
				sun->setMinute(0);
			}
            if(glfwGetKey(m_context->getWindow(), GLFW_KEY_I))
            {
                scene::SceneManager::instance()->getLight(0)->getTransform()->translate(0.0, 1.0f, 0.0f);
            }
            if(glfwGetMouseButton(m_context->getWindow(), GLFW_MOUSE_BUTTON_3))
            {
              scroll = 60.0;
            }
            //! Field of view
            m_scene_camera->SetFOV(scroll);

            //! Other keyboard events
            if (glfwGetKey(m_context->getWindow(), GLFW_KEY_1) )
            {
                  m_shaderProgram_forward->reloadAllShaders();
            }
            if (glfwGetKey(m_context->getWindow(), GLFW_KEY_2) )
            {
                m_shaderProgram_compositing->reloadAllShaders();
            }

			if (glfwGetKey(m_context->getWindow(), GLFW_KEY_E) )
            {
				switchExtractionStrategy();
            }

            //! Normal camera mode
            glm::mat4 view       = m_scene_camera->GetViewMatrix();
            glm::mat4 projection = m_scene_camera->GetProjectionMatrix();

            glfwSetScrollCallback(m_context->getWindow(), ScrollCallback);
            glfwSetKeyCallback(m_context->getWindow(), KeyboardCallback);

            //scene::SceneManager::instance()->getLight(0)->generateShadowMap(&m_renderqueue);

            //! First shader program:
            //! ### GEOMETRY RENDER ############################################
            gBuffer->write();
			
				glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

				

					double forwardTime1 = glfwGetTime();
					//! First shader program
				
				//render sky
				m_shaderProgram_sky->use();

					//uniforms
					sun->update(projection,view);
					m_shaderProgram_sky->setUniform(sky_uniform_loc_view, view);
					m_shaderProgram_sky->setUniform(sky_uniform_loc_model, skyScale);
					m_shaderProgram_sky->setUniform(sky_uniform_loc_projection, projection);
					m_shaderProgram_sky->setUniformSampler(sky_uniform_loc_day_tex, day_tex, 0); //sonne
					m_shaderProgram_sky->setUniformSampler(sky_uniform_loc_night_tex, night_tex, 1 ); //sonne
					m_shaderProgram_sky->setUniform(sky_uniform_loc_blend, sun->textureBlend);  //sonne
					m_shaderProgram_sky->setUniform(sky_uniform_loc_color, sun->getColor() ); //sonne
					skyNode->drawTriangles();

				m_shaderProgram_forward->use();
					//m_shaderProgram_forward->SetUniform(uniform_loc_light_position, scene::SceneManager::instance()->getLight(0)->getTransform()->getPosition() );
					m_shaderProgram_forward->setUniform(forward_uniform_loc_mouse, glm::vec2(mouse_correct_x, mouse_correct_y) );
					m_shaderProgram_forward->setUniform(forward_uniform_loc_view, view);
					m_shaderProgram_forward->setUniform(forward_uniform_loc_projection, projection);
					glm::vec3 p = sun->getTransform()->getPosition();
					m_shaderProgram_forward->setUniform(forward_uniform_loc_light_position, sun->getTransform()->getPosition());
					m_shaderProgram_forward->setUniform(forward_uniform_loc_light_color, sun->getColor());

					//render geometry nodes 
					for(unsigned int i = 0; i < m_renderqueue.size(); i++)
					{
						m_shaderProgram_forward->setUniform(forward_uniform_loc_model, m_renderqueue[i]->getTransform()->getModelMatrix() );
						m_shaderProgram_forward->setUniform(forward_uniform_loc_diffuse_color, *(m_renderqueue[i]->getMaterial()->getDiffuseColor()) );
						m_shaderProgram_forward->setUniform(forward_uniform_loc_specular_color, *(m_renderqueue[i]->getMaterial()->getSpecularColor()) );
						m_shaderProgram_forward->setUniform(forward_uniform_loc_shininess, m_renderqueue[i]->getMaterial()->getShininess() );
						m_shaderProgram_forward->setUniformSampler(forward_uniform_loc_diffuse_tex, m_renderqueue[i]->getMaterial()->getDiffuseTexture(), 0);
						m_shaderProgram_forward->setUniformSampler(forward_uniform_loc_specular_tex, m_renderqueue[i]->getMaterial()->getSpecularTexture(), 1);
						m_shaderProgram_forward->setUniformSampler(forward_uniform_loc_normal_tex, m_renderqueue[i]->getMaterial()->getNormalTexture(), 2);
						m_renderqueue[i]->drawTriangles();
					}


				m_shaderProgram_forward->unuse();

			double forwardTime2 = glfwGetTime() - forwardTime1; //ca 1-10ms

			m_framecount++;

			/*ssSunPos = projection * view * wsSunPos;
			ssSunPos.x=(ssSunPos.x/ssSunPos.z)/2.0f+0.5f;
			ssSunPos.y=(ssSunPos.y/ssSunPos.z)/2.0f+0.5f;*/
			
			double time1 = glfwGetTime();
			doTheSunlightEffect();
			double time2 = glfwGetTime() - time1; //ca 1,3*e-5 

			compositingPass->doExecute();

			time1 = glfwGetTime();
			extractionPass->doExecute();
			time2 = glfwGetTime() - time1; // bla*e-6
			//der langsame mode dauert 10-20ms



			if (finalPass->param_fastExtraction == 0.0f)
				finalPass->param_minAveMax = glm::vec3(slowExtractionPass->image_min, slowExtractionPass->image_average, slowExtractionPass->image_max);
			else
				finalPass->minAveMaxTexture = fastExtractionPass->outputTexture;

			finalPass->doExecute();


           
            //! Swap buffers
            m_context->swapBuffers();
        }
    }

    std::string Renderer::getShaderSourceOf(GLSL::GLSLShaderType shaderType)
    {
        std::string code = m_shaderProgram_forward->getShaderCodeOf(shaderType);
        return code;
    }

	void Renderer::doTheSunlightEffect()
	{
		//downsample gbuffer color
		SlimFBO::blit(gBuffer,sunlightFBO0);
		
		//blur horizontally
		blurPass->outputFBO = sunlightFBO1;
		blurPass->inputFBOs[0] = gBuffer;
		blurPass->param_glowHorizontal = 1.0f;
		blurPass->doExecute();

		//switch fbos
		//blur vertically
		blurPass->outputFBO = sunlightFBO2;
		blurPass->inputFBOs[0] = sunlightFBO1;
		blurPass->param_glowHorizontal = 0.0f;
		blurPass->doExecute();

		//calculate the radialMask
		maskPass->param_ssSunPos = glm::vec4(sun->ssPos,1);
		maskPass->doExecute();

		//calculate Luminace
		luminancePass->param_ssSunPos=glm::vec4(sun->ssPos,1);
		luminancePass->doExecute();
	}


	void Renderer::switchExtractionStrategy()
	{
		MipMapExtractionPass* ex = dynamic_cast<MipMapExtractionPass*>(extractionPass);
		if (ex != 0) {
			//extractionPass is MipMap change to CPU
			extractionPass = slowExtractionPass;
			finalPass->minAveMaxTexture = 0;
			finalPass->param_fastExtraction = 0.0f;
		}
		else
		{
			extractionPass = fastExtractionPass;
			finalPass->minAveMaxTexture = fastExtractionPass->outputTexture;
			finalPass->param_fastExtraction = 1.0f;
		}
	}

}
