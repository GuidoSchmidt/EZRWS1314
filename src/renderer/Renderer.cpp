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

    utils::Importer::instance()->
    importFile(RESOURCES_PATH "/scenes/dae/head.dae", "head");
    m_renderqueue = scene::SceneManager::instance()->generateRenderQueue();

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
        GLSL::VERTEX, RESOURCES_PATH "/shader_source/translucency.vert.glsl",
        GLSL::FRAGMENT, RESOURCES_PATH "/shader_source/translucency.frag.glsl");

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

void KeyboardCallback(GLFWwindow* window, int key, int scancode, int action,
                      int mods)
{

}

void Renderer::KeyboardCheck(void)
{
    //! simple camera movement
    float camera_speed = 0.015f;
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

    //!  Moving light
    if (glfwGetKey(m_context->getWindow(), GLFW_KEY_UP))
    {
        scene::SceneManager::instance()->getLight(0)->
                             getTransform()->translate(0.0, 0.05, 0.0);
    }
    if (glfwGetKey(m_context->getWindow(), GLFW_KEY_DOWN))
    {
        scene::SceneManager::instance()->getLight(0)->
                             getTransform()->translate(0.0, -0.05, 0.0);
    }
    if (glfwGetKey(m_context->getWindow(), GLFW_KEY_RIGHT))
    {
        scene::SceneManager::instance()->getLight(0)->
                             getTransform()->translate(0.05, 0.0, 0.0);
    }
    if (glfwGetKey(m_context->getWindow(), GLFW_KEY_LEFT))
    {
        scene::SceneManager::instance()->getLight(0)->
                             getTransform()->translate(-0.05, 0.00, 0.0);
    }
    if (glfwGetKey(m_context->getWindow(), GLFW_KEY_E))
    {
        scene::SceneManager::instance()->getLight(0)->
                             getTransform()->translate(0.0, 0.00, 0.05);
    }
    if (glfwGetKey(m_context->getWindow(), GLFW_KEY_R))
    {
        scene::SceneManager::instance()->getLight(0)->
                             getTransform()->translate(0.0, 0.05, -0.05);
    }


    if(glfwGetMouseButton(m_context->getWindow(), GLFW_MOUSE_BUTTON_3))
    {
        scroll = 60.0;
    }
    if (glfwGetKey(m_context->getWindow(), GLFW_KEY_Q))
    {
        exit(0);
    }
    //! Print camera pose
    if (glfwGetKey(m_context->getWindow(), GLFW_KEY_P))
    {
        std::cout << "-----------------------------------------------------"
                  << std::endl;
        std::cout << "Scene Camera Pose: ("
                  << m_scene_camera->GetPosition().x << ", "
                  << m_scene_camera->GetPosition().y << ", "
                  << m_scene_camera->GetPosition().z << ", "
                  << m_scene_camera->getTransform()->getRotation().x << ", "
                  << m_scene_camera->getTransform()->getRotation().y << ", "
                  << m_scene_camera->getTransform()->getRotation().z
                  << ")"<< std::endl;

    }

    //! set shader properties
    if(glfwGetKey(m_context->getWindow(), GLFW_KEY_F1))
    {
        if( m_cellLevels >= 2 )
        {
            // to get rid of button polling
            m_cellLevelsKon -= 0.01;
            m_cellLevels = (int)m_cellLevelsKon;
        }
    }
    if(glfwGetKey(m_context->getWindow(), GLFW_KEY_F2))
    {
        m_cellLevelsKon += 0.01;
        m_cellLevels = (int)m_cellLevelsKon;
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
}

void Renderer::renderloop()
{
    float camera_speed = 0.025f;

    //! Render calls here
    m_scene_camera = new scene::Camera(0,"scene_camera",
                                       glm::vec3(1.60502f, 1.79728f, 4.40802f),
                                       glm::vec3(-1.5f, 2.2f, 0.0f),
                                       glm::vec3(0.0f, 1.0f, 0.0f),
                                       m_context->getSize());

    glm::quat camera_rot = m_scene_camera->getTransform()->getRotation();
    glm::vec3 camera_tra = m_scene_camera->getTransform()->getPosition();
    glm::vec3 camera_sca = m_scene_camera->getTransform()->getScale();

    scene::SceneManager::instance()->getLight(0)->setTransform(camera_tra,
                                                               camera_rot,
                                                               camera_sca);

    //! Uniform setup
    //! Standard Uniforms
    GLuint forward_uniform_loc_model            = m_shaderProgram_simple->
            getUniform("model");

    GLuint forward_uniform_loc_view             = m_shaderProgram_simple->
            getUniform("view");

    GLuint forward_uniform_loc_projection       = m_shaderProgram_simple->
            getUniform("projection");

    GLuint forward_uniform_loc_modelview        = m_shaderProgram_simple->
            getUniform("modelview");

    GLuint forward_uniform_loc_normalmatrix     = m_shaderProgram_simple->
            getUniform("normalmatrix");

    GLuint forward_uniform_loc_mvp              = m_shaderProgram_simple->
            getUniform("mvp");

    GLuint forward_uniform_loc_diffuse_tex      = m_shaderProgram_simple->
            getUniform("diffuse_tex");

    //! Phong Shading Uniforms
    GLuint forward_uniform_loc_specular_tex     = m_shaderProgram_simple->
            getUniform("specular_tex");

    GLuint forward_uniform_loc_normal_tex       = m_shaderProgram_simple->
            getUniform("normal_tex");

    GLuint forward_uniform_loc_lightposition    = m_shaderProgram_simple->
            getUniform("LightPosition");

    GLuint forward_uniform_loc_shininess        = m_shaderProgram_simple->
            getUniform("Shininess");

    //! Cel Shading Uniforms
    GLuint forward_uniform_loc_cellevels        = m_shaderProgram_simple->
            getUniform("levels");

    //! Translucency (Colin Barre-Brisebois)
    GLuint forward_uniform_loc_translucency_tex = m_shaderProgram_simple->
            getUniform("translucency_tex");



    // set up some matrices
    glm::mat4 model, view, modelview, projection, mvp;
    glm::mat3 normalmatrix;

    while (m_context && m_context->isLive() && !glfwGetKey(
                m_context->getWindow(), GLFW_KEY_ESCAPE) )
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        KeyboardCheck();

        view       = m_scene_camera->GetViewMatrix();
        projection = m_scene_camera->GetProjectionMatrix();
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

        //! First shader program:
        //! ### GEOMETRY RENDER ############################################
        m_shaderProgram_simple->use();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glCullFace(GL_BACK);
        glPolygonOffset(0.9, 1.0);

        glViewport(0, 0, m_context->getSize().x, m_context->getSize().y);

        m_shaderProgram_simple->setUniform(forward_uniform_loc_view, view);

        m_shaderProgram_simple->setUniform(forward_uniform_loc_projection,
                                           projection);

        glm::vec4 lightPosition = glm::vec4( scene::SceneManager::instance()->
                                                  getLight(0)->
                                                  getTransform()->
                                                  getPosition(),
                                             1.0);

        m_shaderProgram_simple->setUniform(forward_uniform_loc_lightposition,
                                           lightPosition);

        m_shaderProgram_simple->setUniform(forward_uniform_loc_cellevels,
                                           m_cellLevels);

        // passing all the neccesary per fragment information to our shaders
        for(unsigned int i = 0; i < m_renderqueue.size(); i++)
        {
            model = m_renderqueue[i]->getTransform()->getModelMatrix();
            m_shaderProgram_simple->setUniform(forward_uniform_loc_model,
                                               model );

            modelview = view * model;
            m_shaderProgram_simple->setUniform(forward_uniform_loc_modelview,
                                               modelview );

            normalmatrix = glm::mat3( glm::vec3(modelview[0]),
                                      glm::vec3(modelview[1]),
                                      glm::vec3(modelview[2])
                                    );

            m_shaderProgram_simple->setUniform(forward_uniform_loc_normalmatrix,
                                               normalmatrix );

            mvp = projection * modelview;
            m_shaderProgram_simple->setUniform(forward_uniform_loc_mvp,
                                               mvp );

            m_shaderProgram_simple->setUniform(
                forward_uniform_loc_shininess,
                m_renderqueue[i]->getMaterial()->getShininess());

            m_shaderProgram_simple->setUniformSampler(
                forward_uniform_loc_diffuse_tex,
                m_renderqueue[i]->getMaterial()->getDiffuseTexture(),
                0);

            m_shaderProgram_simple->setUniformSampler(
                forward_uniform_loc_specular_tex,
                m_renderqueue[i]->getMaterial()->getSpecularTexture(),
                1);

            m_shaderProgram_simple->setUniformSampler(
                forward_uniform_loc_normal_tex,
                m_renderqueue[i]->getMaterial()->getNormalTexture(),
                2);

            m_shaderProgram_simple->setUniformSampler(
                forward_uniform_loc_translucency_tex,
                m_renderqueue[i]->getMaterial()->getNormalTexture(),
                3);

            m_renderqueue[i]->drawTriangles();
        }

        m_shaderProgram_simple->printActiveUniforms();

        m_shaderProgram_simple->unuse();

        //! Swap buffers
        m_context->swapBuffers();
    }
}
}
