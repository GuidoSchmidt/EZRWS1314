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

    utils::Importer::instance()->importFile(
        RESOURCES_PATH "/scenes/dae/house.dae", "house");

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
    glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_CULL_FACE);
    glEnable(GL_POLYGON_OFFSET_FILL);
}

void Renderer::setupShaderStages()
{
    //! Simple forward rendering
    m_shaderProgram_simple = new ShaderProgram(
        GLSL::VERTEX, RESOURCES_PATH "/shader_source/phong_mk.vert.glsl",
        GLSL::FRAGMENT, RESOURCES_PATH "/shader_source/phong_mk.frag.glsl");

    m_shaderProgram_simple->link();

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

void KeyboardCallback(GLFWwindow* window,
                      int key,
                      int scancode,
                      int action,
                      int mods)
{

}

void Renderer::renderloop()
{
    //! Render calls here
    m_scene_camera = new scene::Camera(0,"scene_camera",
                                       glm::vec3(-3.51f, 7.0f, -14.55f),
                                       glm::vec3(0.0f, 2.0f, 0.0f),
                                       glm::vec3(0.0f, 1.0f, 0.0f),
                                       m_context->getSize());

    //! Uniform setup
    GLuint forward_uniform_loc_model        = m_shaderProgram_simple->
            getUniform("Model");

    GLuint forward_uniform_loc_view         = m_shaderProgram_simple->
            getUniform("View");

    GLuint forward_uniform_loc_projection   = m_shaderProgram_simple->
            getUniform("Projection");

    GLuint forward_uniform_loc_mvp          = m_shaderProgram_simple->
            getUniform("MVP");

    GLuint forward_uniform_loc_normalmatrix = m_shaderProgram_simple->
            getUniform("NormalMatrix");

    GLuint forward_uniform_loc_diffuse_tex  = m_shaderProgram_simple->
            getUniform("diffuse_map");

    float camera_speed = 0.025f;

    scene::SceneManager::instance()->getLight(0)->setupShadowMapping(
        glm::vec2(1024.0));

    while (m_context && m_context->isLive() &&
            !glfwGetKey(m_context->getWindow(), GLFW_KEY_ESCAPE) )
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        //! simple camera movement
        double mouse_x, mouse_y;
        float  mouse_correct_x, mouse_correct_y;
        glfwGetCursorPos(m_context->getWindow(), &mouse_x, &mouse_y);
        mouse_correct_x = ((mouse_x / m_context->getSize().x) *2.0f) - 1.0f;
        mouse_correct_y = ((mouse_y / m_context->getSize().y) *2.0f) - 1.0f;

        if (glfwGetMouseButton(m_context->getWindow(), GLFW_MOUSE_BUTTON_2))
        {
            m_scene_camera->Rotate(mouse_correct_x * camera_speed *100.0f,
                                   mouse_correct_y * camera_speed*100.0f);
        }

        //!  Moving camera
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
        if (glfwGetKey(m_context->getWindow(), GLFW_KEY_1) )
        {
            m_shaderProgram_simple->reloadAllShaders();
        }
        if (glfwGetKey(m_context->getWindow(), GLFW_KEY_2) )
        {
            m_shaderProgram_simple->reloadAllShaders();
        }



        glm::mat4 view       = m_scene_camera->GetViewMatrix();
        glm::mat4 projection = m_scene_camera->GetProjectionMatrix();

        if (glfwGetKey(m_context->getWindow(), GLFW_KEY_0) )
        {
            view       = m_scene_camera->GetViewMatrix();
            projection = m_scene_camera->GetProjectionMatrix();
        }

        if (glfwGetKey(m_context->getWindow(), GLFW_KEY_9) )
        {
            view       = scene::SceneManager::instance()->getLight(0)->
                         getViewMatrix();

            projection = scene::SceneManager::instance()->getLight(0)->
                         getProjectionMatrix();
        }

        //! Normal camera mode
        glfwSetScrollCallback(m_context->getWindow(), ScrollCallback);
        glfwSetKeyCallback(m_context->getWindow(), KeyboardCallback);

        scene::SceneManager::instance()->getLight(0)->generateShadowMap(
            &m_renderqueue);

        //! Shader program:
        //! ### GEOMETRY RENDER ############################################
        m_shaderProgram_simple->use();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glCullFace(GL_BACK);
        glPolygonOffset(0.9, 1.0);

        glViewport(0, 0, m_context->getSize().x, m_context->getSize().y);

        m_shaderProgram_simple->setUniform(forward_uniform_loc_view, view);

        m_shaderProgram_simple->setUniform(forward_uniform_loc_projection,
                                           projection);


        for(unsigned int i = 0; i < m_renderqueue.size(); i++)
        {
            m_shaderProgram_simple->setUniform(forward_uniform_loc_model,
                                               m_renderqueue[i]->
                                               getTransform()->
                                               getModelMatrix() );

            m_shaderProgram_simple->setUniformSampler(
                forward_uniform_loc_diffuse_tex,
                m_renderqueue[i]->getMaterial()->
                getDiffuseTexture(),
                0);

            m_renderqueue[i]->drawTriangles();
        }

        m_shaderProgram_simple->unuse();

        //! Swap buffers
        m_context->swapBuffers();
    }
}
}
