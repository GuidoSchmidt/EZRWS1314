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
        setupShaderStages();
    
        //! \todo Loads models via utils::Importer
        utils::Importer::instance()->importFile(RESOURCES_PATH "/scenes/dae/head.dae");
        m_renderqueue = scene::SceneManager::instance()->generateRenderQueue();

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
        //! Simple forward rendering
        m_shaderProgram_forward = new ShaderProgram(GLSL::VERTEX, RESOURCES_PATH "/shader/forward/forward.vs.glsl",
                                                    GLSL::FRAGMENT, RESOURCES_PATH "/shader/forward/forward.fs.glsl");
        m_shaderProgram_forward->link();
        m_fbo = new FrameBufferObject(m_context->getSize().x, m_context->getSize().y);
        m_fbo->addColorAttachment(0);
        m_fbo->addDepthAttachment_Texture(1);

        //! Compositing rendering on fullscreen quad
        m_shaderProgram_compositing = new ShaderProgram(GLSL::VERTEX, RESOURCES_PATH "/shader/compositing/fullscreen.vs.glsl",
                                                        GLSL::FRAGMENT, RESOURCES_PATH "/shader/compositing/compositing.fs.glsl");
        m_shaderProgram_compositing->link();
        m_fullscreen_triangle = new utils::FullscreenTriangle();
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
        GLuint forward_uniform_loc_mouse            = m_shaderProgram_forward->getUniform("mouse");
        //! Compositing
        GLuint compositing_uniform_loc_shadowMap    = m_shaderProgram_compositing->getUniform("shadowMap");
        GLuint compositing_uniform_loc_lightedMap   = m_shaderProgram_compositing->getUniform("lightedMap");

        glm::vec3 camera_position = glm::vec3(1.0f);
        float camera_speed = 0.01f;

        scene::SceneManager::instance()->getLight(0)->setupShadowMapping(glm::vec2(512));

        while (m_context && m_context->isLive() && !glfwGetKey(m_context->getWindow(), GLFW_KEY_ESCAPE) )
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

            //! Normal camera mode
            glm::mat4 view       = m_scene_camera->GetViewMatrix();
            glm::mat4 projection = m_scene_camera->GetProjectionMatrix();

            glfwSetScrollCallback(m_context->getWindow(), ScrollCallback);
            glfwSetKeyCallback(m_context->getWindow(), KeyboardCallback);

            scene::SceneManager::instance()->getLight(0)->generateShadowMap(&m_renderqueue);

            //! First shader program:
            //! ### GEOMETRY RENDER ############################################
            m_shaderProgram_forward->use();
            m_fbo->use();
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            glViewport(0, 0, m_context->getSize().x, m_context->getSize().y);

            //m_shaderProgram_forward->SetUniform(uniform_loc_light_position, scene::SceneManager::instance()->getLight(0)->getTransform()->getPosition() );
            m_shaderProgram_forward->setUniform(forward_uniform_loc_mouse, glm::vec2(mouse_correct_x, mouse_correct_y) );
            m_shaderProgram_forward->setUniform(forward_uniform_loc_view, view);
            m_shaderProgram_forward->setUniform(forward_uniform_loc_projection, projection);

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
            m_fbo->unuse();


            //! Second shader program:
            //! ### FULLSCREEN TRIANGLE ########################################
            m_shaderProgram_compositing->use();
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            //! \todo 15.01.14 FBO-textures does not work yet
            m_shaderProgram_compositing->setUniformSampler(compositing_uniform_loc_lightedMap, m_fbo->getTexture(0), 0);
            m_shaderProgram_compositing->setUniformSampler(compositing_uniform_loc_shadowMap, scene::SceneManager::instance()->getLight(0)->getShadowMap(), 1);

            m_fullscreen_triangle->draw();
            //m_renderqueue[0]->drawTriangles();

            m_shaderProgram_compositing->unuse();

            //! Swap buffers
            m_context->swapBuffers();
        }
    }
}
