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

	
		//ToDo Modells laden
		//ToDo Texturen laden
		//Gui bauen

        setupShaderStages();
    
        //! \todo Loads models via utils::Importer
        //utils::Importer::instance()->importFile(RESOURCES_PATH "/scenes/dae/simple_cube.dae");
		utils::Importer::instance()->importFile(RESOURCES_PATH "/scenes/obj/baum_test.obj");
		utils::Importer::instance()->importFile(RESOURCES_PATH "/scenes/obj/head.obj");
		//utils::Importer::instance()->importFile(RESOURCES_PATH "/scenes/dae/baum_test.dae");
        //utils::Importer::instance()->importFile(RESOURCES_PATH "/scenes/dae/simple_cube.dae");


        //! \todo Load textures (should be done by the utils::importer-class) using a class that will manage textures and materials
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


		slowExtractionPass = new CPUExtractionPass(fsq, WIDTH, HEIGHT);
		slowExtractionPass->inputFBOs.push_back(gBuffer);
		slowExtractionPass->inputFBOs.push_back(downsampledExtractionFBO);

		fastExtractionPass = new MipMapExtractionPass(fsq, WIDTH, HEIGHT);
		fastExtractionPass->inputFBOs.push_back(gBuffer);

		extractionPass = fastExtractionPass;

		
		finalPass = new FinalPass(fsq, WIDTH, HEIGHT);
		finalPass->outputFBO = lightingFBO;
		finalPass->inputFBOs.push_back(gBuffer);
		//finalPass->inputFBOs.push_back(sunlightFBO1);
		finalPass->inputFBOs.push_back(sunlightFBO2);
		finalPass->inputFBOs.push_back(sunlightFBO3);
		finalPass->inputFBOs.push_back(sunlightFBO4);




        //phong1 = new PhongPass(fsq, nearFar,WIDTH,HEIGHT);//,mouseX,mouseY);
        //phong1->outputFBO = lightingFBO;
        //phong1->inputFBOs.push_back(gBuffer);

        //glowHalf = new GlowPass(1,fsq,WIDTH,HEIGHT);
        //glowHalf->outputFBO = glowFBO;
        //glowHalf->inputFBOs.push_back(lightingFBO);
    
		
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
        m_shaderProgram_forward = new ShaderProgram(GLSL::VERTEX, RESOURCES_PATH "/shader/forward/forward.vs.glsl",
                                                    GLSL::FRAGMENT, RESOURCES_PATH "/shader/forward/forward.fs.glsl");
        m_shaderProgram_forward->Link();
    }

    void Renderer::renderloop()
    {
		//! \todo Exclude texture loading to class 'TextureManager'
		//! Load a texture
		delta = glfwGetTime()-delta;
		sunSpeed = 1000.0;
		sunRadius = 100.0;
		sunAngle = 0.0;
		GLuint textureUV = utils::Importer::instance()->loadTexture(RESOURCES_PATH "/textures/common/uv_test.jpg", true);
		GLuint leaf_tex = utils::Importer::instance()->loadTexture(RESOURCES_PATH "/textures/Leaf08.png", true);
		GLuint head_tex = utils::Importer::instance()->loadTexture(RESOURCES_PATH "/textures/head.jpg", true);
		//GLuint texture_handle2 = utils::Importer::instance()->loadTexture(RESOURCES_PATH "/textures/sky_test.jpg");
		GLuint trunk_tex = utils::Importer::instance()->loadTexture(RESOURCES_PATH "/textures/Wood01.png",true);
		GLuint sky_tex = utils::Importer::instance()->loadTexture(RESOURCES_PATH "/textures/beach_small.jpg",true);
		//GLuint sky_tex = utils::Importer::instance()->loadHDRTexture(RESOURCES_PATH "/textures/beach_small.exr");
		GLuint ldr_diffuse_cube = utils::Importer::instance()->loadCubeMap(RESOURCES_PATH "/textures/ldr-cross/beach_small_diffuse_cross", false);
		GLuint ldr_reflective_cube = utils::Importer::instance()->loadCubeMap(RESOURCES_PATH "/textures/ldr-cross/beach_small_reflective_cross", false);

        //! Render calls here
		scene::Geometry* suzanne = utils::Importer::instance()->getGeometryNode(0); // suzanne
		scene::Geometry* sky = utils::Importer::instance()->getGeometryNode(1);
		scene::Geometry* trunk = utils::Importer::instance()->getGeometryNode(2);
		scene::Geometry* trunk2 = utils::Importer::instance()->getGeometryNode(3); 
		scene::Geometry* leafs = utils::Importer::instance()->getGeometryNode(4);
		scene::Geometry* head = utils::Importer::instance()->getGeometryNode(25);


		glm::mat4 model = sky->getTransform()->getModelMatrix();

		scene::Camera* camera0 = new scene::Camera("scene_camera",	
													glm::vec3(0.0f, 10.0f, -20.0f),
													glm::vec3(0.0f, 10.0f, 0.0f),
													glm::vec3(0.0f, 1.0f, 0.0f),
													m_context->getSize());
		camera0->SetFarPlane(10000.0f);
		glm::mat4 projection = camera0->GetProjectionMatrix();


		glm::vec3 camera_position = glm::vec3(1.0f);
		float camera_speed = 1;
        while (m_context && m_context->isLive())
        {
			delta = glfwGetTime() - delta;
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

			gBuffer->write();


				//! simple camera movement
				double mouse_x, mouse_y;
				float  mouse_correct_x, mouse_correct_y;
				glfwGetCursorPos(m_context->getWindow(), &mouse_x, &mouse_y);
				mouse_correct_x = ((mouse_x / m_context->getSize().x) * 2.0f) - 1.0f;
				mouse_correct_y = ((mouse_y / m_context->getSize().y) * 2.0f) - 1.0f;
				if (glfwGetMouseButton(m_context->getWindow(), GLFW_MOUSE_BUTTON_2))
				{
					camera0->Rotate(mouse_correct_x * camera_speed * 10.0f,
									mouse_correct_y * camera_speed * 10.0f);
				}
				if (glfwGetKey(m_context->getWindow(), GLFW_KEY_W) ||
					glfwGetKey(m_context->getWindow(), GLFW_KEY_UP))
				{
					camera0->MoveZ( camera_speed);
				}
				if (glfwGetKey(m_context->getWindow(), GLFW_KEY_S) ||
					glfwGetKey(m_context->getWindow(), GLFW_KEY_DOWN))
				{
					camera0->MoveZ(-camera_speed);
				}
				if (glfwGetKey(m_context->getWindow(), GLFW_KEY_D) ||
					glfwGetKey(m_context->getWindow(), GLFW_KEY_RIGHT))
				{
					camera0->MoveX( camera_speed);
				}
				if (glfwGetKey(m_context->getWindow(), GLFW_KEY_A) ||
					glfwGetKey(m_context->getWindow(), GLFW_KEY_LEFT))
				{
					camera0->MoveX(-camera_speed);
				}
				if (glfwGetKey(m_context->getWindow(), GLFW_KEY_E))
				{
					switchExtractionStrategy();
				}
				glm::mat4 view = camera0->GetViewMatrix();


				//! First shader program
				m_shaderProgram_forward->Use();
			
					m_shaderProgram_forward->SetUniform("model", model);
					m_shaderProgram_forward->SetUniform("sky", 0.0f);
					m_shaderProgram_forward->SetUniform("view", view);
					m_shaderProgram_forward->SetUniform("projection", projection);
					m_shaderProgram_forward->SetUniform("wsCamPosition", camera0->GetPosition());

					m_shaderProgram_forward->SetUniformCubemap("diffuse_cube", ldr_diffuse_cube, 1);
					m_shaderProgram_forward->SetUniformCubemap("reflective_cube", ldr_reflective_cube, 2);
					m_shaderProgram_forward->SetUniformSampler("material_texture", leaf_tex, 0);
					leafs->drawTriangles();
					m_shaderProgram_forward->SetUniformSampler("material_texture", trunk_tex, 0);
					trunk->drawTriangles();
					trunk2->drawTriangles();
					suzanne->drawTriangles();
					//trunk->drawTriangles();
					m_shaderProgram_forward->SetUniformSampler("material_texture", head_tex, 0);
					head->drawTriangles();
					m_shaderProgram_forward->SetUniform("sky", 1.0f);
					m_shaderProgram_forward->SetUniformSampler("material_texture", sky_tex, 0);
					sky->drawTriangles();
					//render sky

			    m_shaderProgram_forward->Unuse();

			m_framecount++;

			wsSunPos = glm::vec4(0, 10, 100, 1.0);
			sunAngle += delta / sunSpeed;
			//wsSunPos.x = cos(sunAngle)*sunRadius;
			//wsSunPos.z = sin(sunAngle)*sunRadius;
			ssSunPos = projection * view * wsSunPos;
			ssSunPos.x=(ssSunPos.x/ssSunPos.z)/2.0f+0.5f;
			ssSunPos.y=(ssSunPos.y/ssSunPos.z)/2.0f+0.5f;
			
			double time1 = glfwGetTime();
			doTheSunlightEffect();
			double time2 = glfwGetTime() - time1; //< 1ms naaaiisee

			time1 = glfwGetTime();
			//fastExtractionPass->inputTexture = textureUV;
			extractionPass->doExecute();
			time2 = glfwGetTime() - time1; // 



			MipMapExtractionPass* ex = dynamic_cast<MipMapExtractionPass*>(extractionPass);
			if (ex != 0) 
				finalPass->minAveMaxTexture = ex->outputTexture;
			else
			{
				CPUExtractionPass* ex = dynamic_cast<CPUExtractionPass*>(extractionPass);
				if (ex != 0)
					finalPass->param_minAveMax = glm::vec3(ex->image_min, ex->image_average, ex->image_max);	
			}

			finalPass->doExecute();

			m_context->swapBuffers();
        }
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
		maskPass->param_ssSunPos=ssSunPos;
		maskPass->doExecute();

		//calculate Luminace
		luminancePass->param_ssSunPos=ssSunPos;
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
