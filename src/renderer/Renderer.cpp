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
        setupGL();
        setupShaderStages();
    
        //! \todo Loads models via utils::Importer
        utils::Importer::instance()->importFile(RESOURCES_PATH "/scenes/dae/untitled.dae");

        //! \todo Load textures (should be done by the utils::importer-class) using a class that will manage textures and materials
        //! \todo Create user interface

        //! \todo Use Context::getSize().x and .y!
        int WIDTH = m_context->getSize().x;
        int HEIGHT = m_context->getSize().y;
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
    
        renderloop();
    }
    
    void Renderer::setupGL(void)
    {
        //! OpenGL settings
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glEnable(GL_DEPTH_TEST);
    }

    void Renderer::setupShaderStages()
    {
        m_shaderProgram_forward = new ShaderProgram(GLSL::VERTEX, RESOURCES_PATH "/shader/forward/forward.vs.glsl",
                                                    GLSL::FRAGMENT, RESOURCES_PATH "/shader/forward/forward.fs.glsl");
        m_shaderProgram_forward->Link();
    }

    void Renderer::renderloop()
    {
        scene::Geometry* node0 = utils::Importer::instance()->getGeometryNode(1);
		glm::mat4 model = node0->getTransform()->getModelMatrix();

		scene::Camera* camera0 = //utils::Importer::instance()->getCameraNode(0);
		new scene::Camera(m_context->getSize());
		camera0->SetPosition(glm::vec3(0.0f, 0.0f, -0.9f));
		camera0->SetLookAt(glm::vec3(0.0f));
		camera0->SetUp(glm::vec3(0.0f, 0.1f, 0.0f));
		camera0->SetFOV(60.0f);
		camera0->SetNearPlane(0.1f);
		camera0->SetFarPlane(100.0f);

		glm::mat4 projection = camera0->GetProjectionMatrix();

		glm::vec3 camera_position = glm::vec3(1.0f);
		float camera_speed = 0.0005f;
        while (m_context && m_context->isLive())
        {
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);


			//! simple camera movement
			if (glfwGetKey(m_context->getWindow(), GLFW_KEY_W) ||
				glfwGetKey(m_context->getWindow(), GLFW_KEY_UP))
			{
				camera_position.z += camera_speed;
				camera0->MoveY( camera_speed);
			}
			if (glfwGetKey(m_context->getWindow(), GLFW_KEY_S) ||
				glfwGetKey(m_context->getWindow(), GLFW_KEY_DOWN))
			{
				camera_position.z -= camera_speed;
				camera0->MoveY(-camera_speed);
			}
			if (glfwGetKey(m_context->getWindow(), GLFW_KEY_D) ||
				glfwGetKey(m_context->getWindow(), GLFW_KEY_RIGHT))
			{
				camera_position.x += camera_speed;
				camera0->MoveX( camera_speed);
			}
			if (glfwGetKey(m_context->getWindow(), GLFW_KEY_A) ||
				glfwGetKey(m_context->getWindow(), GLFW_KEY_LEFT))
			{
				camera_position.x -= camera_speed;
				camera0->MoveX(-camera_speed);
			}
			glm::mat4 view = glm::lookAt(camera_position, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f)); // camera0->GetViewMatrix();


			//! First shader program
            m_shaderProgram_forward->Use();
			
                m_shaderProgram_forward->SetUniform("model", model);
                m_shaderProgram_forward->SetUniform("view", view);
                m_shaderProgram_forward->SetUniform("projection", projection);

                node0->drawTriangles();

           m_shaderProgram_forward->Unuse();

            m_context->swapBuffers();
            m_framecount++;
        }
    }
}
