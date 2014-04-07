//! @file Renderer.cpp
#include <algorithm>

#include "Renderer.h"

double scroll = 32.0;
extern GLFWwindow *glfwindow;


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

    Renderer* Renderer::instance()
    {
        static Renderer m_instance;
        //m_instance.setRenderContext(context);
        return &m_instance;
    }

    void Renderer::setRenderContext(Context& context)
    {
        m_context = &context;
    }

    void Renderer::init(GLFWwindow *window)
    {
		std::cout << "Renderer.init() called" << std::endl;
		setupGL();
		setupShaderStages();

		utils::Importer::instance()->importFile(RESOURCES_PATH "/scenes/obj/sky.obj", "sky");
		skyNode = utils::Importer::instance()->getGeometryNode(0);
		utils::Importer::instance()->deleteGeometryNode(0);
		scene::SceneManager::instance()->deleteGeometryNode(0);

		utils::Importer::instance()->importFile(RESOURCES_PATH "/scenes/dae/house.dae", "house");
		/*
		//utils::Importer::instance()->importFile(RESOURCES_PATH "/scenes/obj/BeachTest.obj", "beach");
		//utils::Importer::instance()->importFile(RESOURCES_PATH "/scenes/dae/head.dae", "house");

		//utils::Importer::instance()->importFile(RESOURCES_PATH "/scenes/dae/baum_test.dae");
		//utils::Importer::instance()->importFile(RESOURCES_PATH "/scenes/dae/simple_cube.dae");

		//utils::Importer::instance()->importFile(RESOURCES_PATH "/scenes/dae/head.dae");
		//m_renderqueue = scene::SceneManager::instance()->generateRenderQueue();

		//! \todo Create user interface

		//! \todo Use Context::getSize().x and .y!
		int WIDTH = 1024;
		int HEIGHT = 768;
		//glm::vec2 nearFar = glm::vec2(0.1,.0);

		//Setup dat slim fboooooos

		gBuffer = new SlimFBO(WIDTH, HEIGHT, 2, true, GL_LINEAR);
		sunlightFBO0 = new SlimFBO(WIDTH / 4, HEIGHT / 4, 1, false, GL_LINEAR);
		sunlightFBO1 = new SlimFBO(WIDTH / 4, HEIGHT / 4, 1, false, GL_LINEAR);
		sunlightFBO2 = new SlimFBO(WIDTH / 4, HEIGHT / 4, 1, false, GL_LINEAR);
		sunlightFBO3 = new SlimFBO(WIDTH / 4, HEIGHT / 4, 1, false, GL_LINEAR);
		sunlightFBO4 = new SlimFBO(WIDTH / 4, HEIGHT / 4, 1, false, GL_LINEAR);

		compositingFBO = new SlimFBO(WIDTH, HEIGHT, 1, false, GL_LINEAR);

		downsampledExtractionFBO = new SlimFBO(WIDTH / 4, HEIGHT / 4, 1, false, GL_LINEAR);

		//now the render passses!
		fsq = new SlimQuad();

		//now the render passses!
		blurPass = new SeparatedBlurPass(fsq, WIDTH / 4, HEIGHT / 4);
		blurPass->inputFBOs.push_back(gBuffer);

		maskPass = new RadialGlowMaskPass(fsq, WIDTH / 4, HEIGHT / 4);
		maskPass->outputFBO = sunlightFBO3;
		maskPass->inputFBOs.push_back(sunlightFBO2);

		luminancePass = new RadialLuminancePass(fsq, WIDTH / 4, HEIGHT / 4);
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
		skyScale[3][3] = 1;
		scene::Transform trans = scene::Transform(glm::vec3(0), glm::toQuat(glm::mat4(1)), glm::vec3(1));
		GLint sunTex = scene::SceneManager::instance()->loadTexture(RESOURCES_PATH "/textures/niceSun.tga", true);
		//GLint sunTex = scene::SceneManager::instance()->loadTexture(RESOURCES_PATH "/textures/sun.png", true);
		sun = new scene::Sun(1337, "zunLigt", trans, glm::vec3(1), 10, 990, 19, sunTex);
		*/
    }
    
    void Renderer::setupGL(void)
    {
        //! OpenGL settings
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

        m_shaderProgram_sun = new ShaderProgram(GLSL::VERTEX, RESOURCES_PATH "/shader/forward/sun.vs.glsl",
                                                GLSL::FRAGMENT, RESOURCES_PATH "/shader/forward/sun.fs.glsl");
        m_shaderProgram_sun->link();
    }

//! ---- Input Handling --------------------------------------------------------
    void ScrollCallback(GLFWwindow * window, double xoffset, double yoffset)
    {
        if (scroll > 32.0)
            scroll = 32.0;
        scroll += yoffset * 20.0;
    }

    void KeyboardCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
    {
        /*if (key== GLFW_KEY_E)
            Renderer::instance(*/
    }

    double old_x = 0.0;
    double old_y = 0.0;
    double mouse_x = 0.0;
    double mouse_y = 0.0;
    float  mouse_correct_x = 0.0f;
    float  mouse_correct_y = 0.0f;;
    glm::vec3 camera_position = glm::vec3(1.0f);
    float camera_speed = 0.005;

    int WIDTH  = 1024;
    int HEIGHT = 768;

    GLuint night_tex;
    GLuint day_tex;

    GLuint forward_uniform_loc_view;
    GLuint forward_uniform_loc_projection;
    GLuint forward_uniform_loc_model;
    GLuint forward_uniform_loc_diffuse_color;
    GLuint forward_uniform_loc_diffuse_tex;
    GLuint forward_uniform_loc_specular_color;
    GLuint forward_uniform_loc_specular_tex;
    GLuint forward_uniform_loc_shininess;
    GLuint forward_uniform_loc_normal_tex;
    GLuint forward_uniform_loc_light_position;
    GLuint forward_uniform_loc_light_color;
    GLuint forward_uniform_loc_mouse;

	GLuint forward_uniform_loc_ambient_amount;
	GLuint forward_uniform_loc_diffuse_amount;

    GLuint sky_uniform_loc_model;
    GLuint sky_uniform_loc_view;
    GLuint sky_uniform_loc_projection;
    GLuint sky_uniform_loc_day_tex;
    GLuint sky_uniform_loc_night_tex;
    GLuint sky_uniform_loc_blend;
    GLuint sky_uniform_loc_color;

	GLuint sun_uniform_loc_model;
	GLuint sun_uniform_loc_view;
	GLuint sun_uniform_loc_projection;
	GLuint sun_uniform_loc_tex;
	GLuint sun_uniform_loc_color;

    GLuint forward_uniform_loc_shadowMap;
    GLuint forward_uniform_loc_shadowModel;
    GLuint forward_uniform_loc_shadowView;
    GLuint forward_uniform_loc_shadowProjection;
    GLuint compositing_uniform_loc_shadowMap;

    void Renderer::setupRenderer(GLFWwindow* window)
    {
        glfwSetScrollCallback(window, ScrollCallback);
        glfwSetKeyCallback(window, KeyboardCallback);

        glm::vec4 wsSunPos = glm::vec4(0, 10, 2000, 1);

        //GLuint night_tex = scene::SceneManager::instance()->loadTexture(RESOURCES_PATH "/textures/common/night.jpg", true);
        //GLuint day_tex = scene::SceneManager::instance()->loadTexture(RESOURCES_PATH "/textures/common/day.jpg", true);
        //GLuint ldr_diffuse_cube = scene::SceneManager::instance()->loadCubeMap(RESOURCES_PATH "/textures/ldr-cross/beach_small_diffuse_cross", false);
        //GLuint ldr_reflective_cube = scene::SceneManager::instance()->loadCubeMap(RESOURCES_PATH "/textures/ldr-cross/beach_small_reflective_cross", false);

        m_scene_camera = new scene::Camera(0, "scene_camera",
            glm::vec3(2.0f, 2.0f, 2.0f),
            glm::vec3(0.0f, 0.0f, 0.0f),
            glm::vec3(0.0f, 1.0f, 0.0f),
            glm::ivec2(1204, 768));


        //! Uniform setup
        //! Forward shading
        
        forward_uniform_loc_view = m_shaderProgram_forward->getUniform("view");
        forward_uniform_loc_projection = m_shaderProgram_forward->getUniform("projection");
        forward_uniform_loc_model = m_shaderProgram_forward->getUniform("model");
        forward_uniform_loc_diffuse_color = m_shaderProgram_forward->getUniform("diffuse_color");
        forward_uniform_loc_diffuse_tex = m_shaderProgram_forward->getUniform("diffuse_map");
        forward_uniform_loc_specular_color = m_shaderProgram_forward->getUniform("specular_color");
        forward_uniform_loc_specular_tex = m_shaderProgram_forward->getUniform("specular_map");
        forward_uniform_loc_shininess = m_shaderProgram_forward->getUniform("shininess");
        forward_uniform_loc_normal_tex = m_shaderProgram_forward->getUniform("normal_map");
        forward_uniform_loc_light_position = m_shaderProgram_forward->getUniform("light_position");
        forward_uniform_loc_mouse = m_shaderProgram_forward->getUniform("mouse");
        forward_uniform_loc_shadowMap = m_shaderProgram_forward->getUniform("shadow_map");
        forward_uniform_loc_shadowModel = m_shaderProgram_forward->getUniform("light_model");
        forward_uniform_loc_shadowView = m_shaderProgram_forward->getUniform("light_view");
        forward_uniform_loc_shadowProjection = m_shaderProgram_forward->getUniform("light_projection");


        night_tex = scene::SceneManager::instance()->loadTexture(RESOURCES_PATH "/textures/common/night.jpg", true);
        day_tex = scene::SceneManager::instance()->loadTexture(RESOURCES_PATH "/textures/common/day.jpg", true);

        //! Render calls here
        m_scene_camera = new scene::Camera(0, "scene_camera",
            glm::vec3(30.0f, 10.0f, 10.0f),
            glm::vec3(0.0f, 10.0f, 10.0f),
            glm::vec3(0.0f, 1.0f, 0.0f),
            glm::ivec2(1024, 768));
        m_scene_camera->SetFarPlane(3000);
        m_scene_camera->SetNearPlane(0.1);

        //! Uniform setup
        //! Forward shading
        forward_uniform_loc_view = m_shaderProgram_forward->getUniform("view");
        forward_uniform_loc_projection = m_shaderProgram_forward->getUniform("projection");
        forward_uniform_loc_model = m_shaderProgram_forward->getUniform("model");
        forward_uniform_loc_diffuse_color = m_shaderProgram_forward->getUniform("diffuse_color");
        forward_uniform_loc_diffuse_tex = m_shaderProgram_forward->getUniform("diffuse_map");
        forward_uniform_loc_specular_color = m_shaderProgram_forward->getUniform("specular_color");
        forward_uniform_loc_specular_tex = m_shaderProgram_forward->getUniform("specular_map");
        forward_uniform_loc_shininess = m_shaderProgram_forward->getUniform("shininess");
        forward_uniform_loc_normal_tex = m_shaderProgram_forward->getUniform("normal_map");
        forward_uniform_loc_light_position = m_shaderProgram_forward->getUniform("light_position");
        forward_uniform_loc_light_color = m_shaderProgram_forward->getUniform("light_color");
        forward_uniform_loc_ambient_amount = m_shaderProgram_forward->getUniform("ambient_amount");
        forward_uniform_loc_diffuse_amount = m_shaderProgram_forward->getUniform("diffuse_amount");
        forward_uniform_loc_mouse = m_shaderProgram_forward->getUniform("mouse");

        //! Sky shader
        sky_uniform_loc_model = m_shaderProgram_sky->getUniform("model");
        sky_uniform_loc_view = m_shaderProgram_sky->getUniform("view");
        sky_uniform_loc_projection = m_shaderProgram_sky->getUniform("projection");
        sky_uniform_loc_day_tex = m_shaderProgram_sky->getUniform("day_tex");
        sky_uniform_loc_night_tex = m_shaderProgram_sky->getUniform("night_tex");
        sky_uniform_loc_blend = m_shaderProgram_sky->getUniform("blend");
        sky_uniform_loc_color = m_shaderProgram_sky->getUniform("color");

        //! Sun shader
        sun_uniform_loc_model = m_shaderProgram_sun->getUniform("model");
        sun_uniform_loc_view = m_shaderProgram_sun->getUniform("view");
        sun_uniform_loc_projection = m_shaderProgram_sun->getUniform("projection");
        sun_uniform_loc_tex = m_shaderProgram_sun->getUniform("tex");
        sun_uniform_loc_color = m_shaderProgram_sun->getUniform("color");
    }
    

    void Renderer::renderloop(GLFWwindow *window)
    {  
        glm::vec3 camera_position = glm::vec3(1.0f);
        float camera_speed = 1;

        //scene::SceneManager::instance()->getLight(0)->setupShadowMapping(glm::vec2(512));

        double mouse_x, mouse_y, old_x, old_y;
        old_x = old_y = mouse_x = mouse_y = 0;

        //! simple camera movement
        glfwGetCursorPos(glfwindow, &mouse_x, &mouse_y);
        mouse_correct_x = (((mouse_x - old_x) / m_context->getSize().x));// * 2.0f) - 1.0f;
        mouse_correct_y = (((mouse_y - old_y) / m_context->getSize().y));// * 2.0f) - 1.0f;
        if (glfwGetMouseButton(glfwindow, GLFW_MOUSE_BUTTON_2))
        {
            m_scene_camera->Rotate(mouse_correct_x * camera_speed * 100,
                mouse_correct_y * camera_speed * 100);
        }

        if (glfwGetKey(glfwindow, GLFW_KEY_W))
        {
            m_scene_camera->MoveZ(camera_speed);
        }
        if (glfwGetKey(glfwindow, GLFW_KEY_S))
        {
            m_scene_camera->MoveZ(-camera_speed);
        }
        if (glfwGetKey(glfwindow, GLFW_KEY_D))
        {
            m_scene_camera->MoveX(camera_speed);
        }
        if (glfwGetKey(glfwindow, GLFW_KEY_A))
        {
            m_scene_camera->MoveX(-camera_speed);
        }
        if (glfwGetKey(glfwindow, GLFW_KEY_UP))
        {
            sun->incHour();
        }
        if (glfwGetKey(glfwindow, GLFW_KEY_DOWN))
        {
            sun->decHour();
        }
        if (glfwGetKey(glfwindow, GLFW_KEY_LEFT))
        {
            sun->decMinute();
        }
        if (glfwGetKey(glfwindow, GLFW_KEY_RIGHT))
        {
            sun->incMinute();
        }
        if (glfwGetKey(glfwindow, GLFW_KEY_O))
        {
            sun->tone_factor += 0.1;
        }
        if (glfwGetKey(glfwindow, GLFW_KEY_I))
        {
            sun->tone_factor -= 0.1;
        }
        if (glfwGetKey(glfwindow, GLFW_KEY_KP_2))
        {
            sun->setHour(0);
            sun->setMinute(0);

        }
        if (glfwGetKey(glfwindow, GLFW_KEY_KP_1))
        {
            sun->setHour(3);
            sun->setMinute(0);
        }
        if (glfwGetKey(glfwindow, GLFW_KEY_KP_4))
        {
            sun->setHour(6);
            sun->setMinute(0);
        }
        if (glfwGetKey(glfwindow, GLFW_KEY_KP_7))
        {
            sun->setHour(9);
            sun->setMinute(0);
        }
        if (glfwGetKey(glfwindow, GLFW_KEY_KP_8))
        {
            sun->setHour(12);
            sun->setMinute(0);
        }
        if (glfwGetKey(glfwindow, GLFW_KEY_KP_9))
        {
            sun->setHour(15);
            sun->setMinute(0);
        }
        if (glfwGetKey(glfwindow, GLFW_KEY_KP_6))
        {
            sun->setHour(18);
            sun->setMinute(0);
        }
        if (glfwGetKey(glfwindow, GLFW_KEY_KP_3))
        {
            sun->setHour(21);
            sun->setMinute(0);
        }
        if (glfwGetMouseButton(glfwindow, GLFW_MOUSE_BUTTON_3))
        {
            scroll = 60.0;
        }
        //! Field of view
        m_scene_camera->SetFOV(scroll);

        //! Other keyboard events
        if (glfwGetKey(glfwindow, GLFW_KEY_1))
        {
            m_shaderProgram_forward->reloadAllShaders();
        }
        if (glfwGetKey(glfwindow, GLFW_KEY_2))
        {
            m_shaderProgram_compositing->reloadAllShaders();
        }
        if (glfwGetKey(glfwindow, GLFW_KEY_E))
        {
            switchExtractionStrategy(true);
        }
        if (glfwGetKey(glfwindow, GLFW_KEY_R))
        {
            switchExtractionStrategy(false);
        }

        //! Normal camera mode
        glm::mat4 view = m_scene_camera->GetViewMatrix();
        glm::mat4 projection = m_scene_camera->GetProjectionMatrix();

        glfwSetScrollCallback(glfwindow, ScrollCallback);
        glfwSetKeyCallback(glfwindow, KeyboardCallback);

        //! First shader program:
        //! ### GEOMETRY RENDER ############################################   
		//gBuffer->write();
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        double forwardTime1 = glfwGetTime();


		/*
        // Set uniforms
        m_shaderProgram_sky->use();
        sun->update(projection, view);
        m_shaderProgram_sky->setUniform(sky_uniform_loc_view, view);
        m_shaderProgram_sky->setUniform(sky_uniform_loc_model, skyScale);
        m_shaderProgram_sky->setUniform(sky_uniform_loc_projection, projection);
        m_shaderProgram_sky->setUniformSampler(sky_uniform_loc_day_tex, day_tex, 0);
        m_shaderProgram_sky->setUniformSampler(sky_uniform_loc_night_tex, night_tex, 1);
        m_shaderProgram_sky->setUniform(sky_uniform_loc_blend, sun->textureBlend);
        m_shaderProgram_sky->setUniform(sky_uniform_loc_color, sun->getColor()*(sun->ambientAmount));
        skyNode->drawTriangles();


        // Draw sun
        m_shaderProgram_sun->use();
        m_shaderProgram_sun->setUniform(sun_uniform_loc_view, view);
        m_shaderProgram_sun->setUniform(sun_uniform_loc_model, sun->getTransform()->getModelMatrix());
        m_shaderProgram_sun->setUniform(sun_uniform_loc_projection, projection);
        m_shaderProgram_sun->setUniformSampler(sun_uniform_loc_tex, sun->sunTexture, 0);
        m_shaderProgram_sun->setUniform(sun_uniform_loc_color, sun->getColor());
        glDisable(GL_DEPTH_TEST);
        glEnable(GL_BLEND);
        glDepthMask(GL_FALSE);
        glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
        sun->render();
        glDepthMask(GL_TRUE);
        glDisable(GL_BLEND);
        glEnable(GL_DEPTH_TEST);
		*/

		// Forward rendering
		/*
        m_shaderProgram_forward->use();
        m_shaderProgram_forward->setUniform(forward_uniform_loc_mouse, glm::vec2(mouse_correct_x, mouse_correct_y));
        m_shaderProgram_forward->setUniform(forward_uniform_loc_view, view);
        m_shaderProgram_forward->setUniform(forward_uniform_loc_projection, projection);
        m_shaderProgram_forward->setUniform(forward_uniform_loc_light_position, sun->getTransform()->getPosition());
        m_shaderProgram_forward->setUniform(forward_uniform_loc_light_color, sun->getColor());
        //m_shaderProgram_forward->setUniform(forward_uniform_loc_ambient_amount, sun->ambientAmount);
        //m_shaderProgram_forward->setUniform(forward_uniform_loc_diffuse_amount, sun->diffuseAmount);
		*/

        //m_shaderProgram_forward->setUniform(forward_uniform_loc_ambient_amount, sun->ambientAmount);
        //m_shaderProgram_forward->setUniform(forward_uniform_loc_diffuse_amount, sun->diffuseAmount);

        //render geometry nodes 
        for (unsigned int i = 0; i < m_renderqueue.size(); i++)
            //for (unsigned int i = 0; i < 0; i++)
        {
            m_shaderProgram_forward->setUniform(forward_uniform_loc_model, m_renderqueue[i]->getTransform()->getModelMatrix());
            m_shaderProgram_forward->setUniform(forward_uniform_loc_diffuse_color, *(m_renderqueue[i]->getMaterial()->getDiffuseColor()));
            m_shaderProgram_forward->setUniform(forward_uniform_loc_specular_color, *(m_renderqueue[i]->getMaterial()->getSpecularColor()));
            m_shaderProgram_forward->setUniform(forward_uniform_loc_shininess, m_renderqueue[i]->getMaterial()->getShininess());
            m_shaderProgram_forward->setUniform(forward_uniform_loc_light_color, sun->getColor());
            m_shaderProgram_forward->setUniformSampler(forward_uniform_loc_diffuse_tex, m_renderqueue[i]->getMaterial()->getDiffuseTexture(), 0);
            m_shaderProgram_forward->setUniformSampler(forward_uniform_loc_specular_tex, m_renderqueue[i]->getMaterial()->getSpecularTexture(), 1);
            m_shaderProgram_forward->setUniformSampler(forward_uniform_loc_normal_tex, m_renderqueue[i]->getMaterial()->getNormalTexture(), 2);
            m_renderqueue[i]->drawTriangles();
        }

        m_shaderProgram_forward->unuse();
        //gBuffer->unbind();
		/*

        double forwardTime2 = glfwGetTime() - forwardTime1; //ca 1-10ms

        m_framecount++;


        double time1 = glfwGetTime();
        doTheSunlightEffect();
        double time2 = glfwGetTime() - time1; //ca 1,3*e-5 

        compositingPass->param_bloomAmount = sun->bloomAmount;
        compositingPass->doExecute();

        time1 = glfwGetTime();
        extractionPass->doExecute();
        time2 = glfwGetTime() - time1; // bla*e-6
        //der langsame mode dauert 10-20ms

        if (finalPass->param_fastExtraction == 0.0f)
            finalPass->param_minAveMax = slowExtractionPass->minAveMax;
        else
            finalPass->minAveMaxTexture = fastExtractionPass->outputTexture;

        finalPass->param_sunColor = sun->getColor();
        finalPass->param_factor = sun->tone_factor;
        finalPass->doExecute();
		*/

		//! Clear all used shader programs
		glUseProgram(0);
    }

    std::string Renderer::getShaderSourceOf(GLSL::GLSLShaderType shaderType, unsigned int &lineCount)
    {
        std::string code = m_shaderProgram_forward->getShaderCodeOf(shaderType, lineCount);
        return code;
    }

    void Renderer::doTheSunlightEffect()
    {
        //downsample gbuffer color52c393ae654a41589ff789a2daff23b16101c5f2
        //SlimFBO::blit(gBuffer,sunlightFBO0);
        
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

        //calculate blooming
        luminancePass->param_ssSunPos=glm::vec4(sun->ssPos,1);
        luminancePass->doExecute();
    }


    void Renderer::switchExtractionStrategy(bool fast)
    {
        if (fast) 
        {
            extractionPass = fastExtractionPass;
            finalPass->minAveMaxTexture = fastExtractionPass->outputTexture;
            finalPass->param_fastExtraction = 1.0f;
        }
        else 
        {
            extractionPass = slowExtractionPass;
            finalPass->minAveMaxTexture = 0;
            finalPass->param_fastExtraction = 0.0f;
        }
    }

}
