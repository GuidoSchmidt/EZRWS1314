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
        utils::Importer::instance()->importFile(RESOURCES_PATH "/scenes/dae/head.dae");
        m_renderqueue = scene::SceneManager::instance()->generateRenderQueue();

        //! \todo Load textures (should be done by the utils::importer-class) using a class that will manage textures and materials
        //! \todo Create user interface

        //! \todo Use Context::getSize().x and .y!
        int WIDTH = m_context->getSize().x;
        int HEIGHT = m_context->getSize().y;
        glm::vec2 nearFar = glm::vec2(0.1,60.0);

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

    void ScrollCallback(GLFWwindow * window, double xoffset, double yoffset)
    {
        scroll += yoffset;
    }

    void Renderer::renderloop()
    {
        //! Render calls here

        m_scene_camera = new scene::Camera(0,"scene_camera",
                           glm::vec3(2.0f, 2.0f, 2.0f),
                           glm::vec3(0.0f, 0.0f, 0.0f),
                           glm::vec3(0.0f, 1.0f, 0.0f),
                           m_context->getSize());

        //! Uniform setup
        GLuint uniform_loc_view             = m_shaderProgram_forward->GetUniform("view");
        GLuint uniform_loc_projection       = m_shaderProgram_forward->GetUniform("projection");
        GLuint uniform_loc_model            = m_shaderProgram_forward->GetUniform("model");
        GLuint uniform_loc_diffuse_color    = m_shaderProgram_forward->GetUniform("diffuse_color");
        GLuint uniform_loc_diffuse_tex      = m_shaderProgram_forward->GetUniform("diffuse_map");
        GLuint uniform_loc_specular_color   = m_shaderProgram_forward->GetUniform("specular_color");
        GLuint uniform_loc_specular_tex     = m_shaderProgram_forward->GetUniform("specular_map");
        GLuint uniform_loc_shininess        = m_shaderProgram_forward->GetUniform("shininess");
        GLuint uniform_loc_normal_tex       = m_shaderProgram_forward->GetUniform("normal_map");


        glm::vec3 camera_position = glm::vec3(1.0f);
        float camera_speed = 0.01f;
        while (m_context && m_context->isLive())
        {
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            //! simple camera movement
            double mouse_x, mouse_y;
            float  mouse_correct_x, mouse_correct_y;
            glfwGetCursorPos(m_context->getWindow(), &mouse_x, &mouse_y);
            mouse_correct_x = ((mouse_x / m_context->getSize().x) * 2.0f) - 1.0f;
            mouse_correct_y = ((mouse_y / m_context->getSize().y) * 2.0f) - 1.0f;
            if (glfwGetMouseButton(m_context->getWindow(), GLFW_MOUSE_BUTTON_2))
            {
                  m_scene_camera->Rotate(mouse_correct_x * camera_speed * 100.0f,
                                         mouse_correct_y * camera_speed * 100.0f);
            }
            if (glfwGetKey(m_context->getWindow(), GLFW_KEY_W) ||
                  glfwGetKey(m_context->getWindow(), GLFW_KEY_UP))
            {
                  m_scene_camera->MoveZ( camera_speed);
            }
            if (glfwGetKey(m_context->getWindow(), GLFW_KEY_S) ||
                  glfwGetKey(m_context->getWindow(), GLFW_KEY_DOWN))
            {
                  m_scene_camera->MoveZ(-camera_speed);
            }
            if (glfwGetKey(m_context->getWindow(), GLFW_KEY_D) ||
                  glfwGetKey(m_context->getWindow(), GLFW_KEY_RIGHT))
            {
                  m_scene_camera->MoveX( camera_speed);
            }
            if (glfwGetKey(m_context->getWindow(), GLFW_KEY_A) ||
                  glfwGetKey(m_context->getWindow(), GLFW_KEY_LEFT))
            {
                  m_scene_camera->MoveX(-camera_speed);
            }
            if(glfwGetMouseButton(m_context->getWindow(), GLFW_MOUSE_BUTTON_3))
            {
              scroll = 60.0;
            }
            //! Field of view
            m_scene_camera->SetFOV(scroll);

            //! Other keyboard events
            if (glfwGetKey(m_context->getWindow(), GLFW_KEY_R) )
            {
                  m_shaderProgram_forward->ReloadAllShaders();
            }

            glm::mat4 view = m_scene_camera->GetViewMatrix();
            glm::mat4 projection = m_scene_camera->GetProjectionMatrix();
            glfwSetScrollCallback(m_context->getWindow(), ScrollCallback);

            //! First shader program
            m_shaderProgram_forward->Use();

            m_shaderProgram_forward->SetUniform(uniform_loc_view, view);
            m_shaderProgram_forward->SetUniform(uniform_loc_projection, projection);

            for(unsigned int i = 0; i < m_renderqueue.size(); i++)
            {
              m_shaderProgram_forward->SetUniform(uniform_loc_model, m_renderqueue[i]->getTransform()->getModelMatrix() );
              m_shaderProgram_forward->SetUniform(uniform_loc_diffuse_color, *(m_renderqueue[i]->getMaterial()->getDiffuseColor()) );
              m_shaderProgram_forward->SetUniform(uniform_loc_specular_color, *(m_renderqueue[i]->getMaterial()->getSpecularColor()) );
              m_shaderProgram_forward->SetUniform(uniform_loc_shininess, m_renderqueue[i]->getMaterial()->getShininess() );
              m_shaderProgram_forward->SetUniformSampler(uniform_loc_diffuse_tex, m_renderqueue[i]->getMaterial()->getDiffuseTexture(), 0);
              m_shaderProgram_forward->SetUniformSampler(uniform_loc_specular_tex, m_renderqueue[i]->getMaterial()->getSpecularTexture(), 1);
              m_shaderProgram_forward->SetUniformSampler(uniform_loc_normal_tex, m_renderqueue[i]->getMaterial()->getNormalTexture(), 2);
              m_renderqueue[i]->drawTriangles();
            }

            m_shaderProgram_forward->Unuse();
            m_context->swapBuffers();
        }
    }
}
