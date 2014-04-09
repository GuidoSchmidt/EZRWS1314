
//! @file Renderer.cpp
#include <algorithm>
#include "Renderer.h"
#include "../utils/Settings.h"


//--- Local Variables ----------------------------------------------------------------------
extern float camera_speed;
extern float move_speed;
double old_x = 0.0;
double old_y = 0.0;
double mouse_x = 0.0;
double mouse_y = 0.0;
float  mouse_correct_x = 0.0f;
float  mouse_correct_y = 0.0f;;
glm::vec3 camera_position = glm::vec3(1.0f);
double scroll = 32.0;
extern GLFWwindow *glfwindow;
extern glm::ivec2 size;

//--- UNIFORM LOCATION NAMES ---------------------------------------------------------------
GLuint night_tex;
GLuint day_tex;
GLuint ocean_tex_diffuse;
GLuint ocean_tex_specular;
GLuint ocean_tex_normal;
GLuint translucent_tex;

GLuint forward_uniform_loc_view;
GLuint forward_uniform_loc_projection;
GLuint forward_uniform_loc_model;
GLuint forward_uniform_loc_diffuse_color;
GLuint forward_uniform_loc_diffuse_tex;
GLuint forward_uniform_loc_specular_color;
GLuint forward_uniform_loc_specular_tex;
GLuint forward_uniform_loc_shininess;
GLuint forward_uniform_loc_normal_tex;
GLuint forward_uniform_loc_translucency;
GLuint forward_uniform_loc_translucent_tex;
GLuint forward_uniform_loc_light_position;
GLuint forward_uniform_loc_light_color;
GLuint forward_uniform_loc_mouse;

GLuint forward_uniform_loc_ambient_amount;
GLuint forward_uniform_loc_diffuse_amount;
GLuint forward_uniform_loc_shadow_amount;

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

GLuint use_tesselation_uniform;

using namespace renderer;

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
	return &m_instance;
}

void Renderer::init(GLFWwindow *window)
{
	std::cout << "Renderer initialized successfully" << std::endl;
	
	setupGL();
	setupShaderStages();

	utils::Importer::instance()->importFile(RESOURCES_PATH "/scenes/dae/sky.dae", "sky");
	skyNode = utils::Importer::instance()->getGeometryNode(0);
	utils::Importer::instance()->deleteGeometryNode(0);
	scene::SceneManager::instance()->deleteGeometryNode(0);

	utils::Importer::instance()->importFile(RESOURCES_PATH "/scenes/dae/ocean.dae", "ocean");
	oceanNode = utils::Importer::instance()->getGeometryNode(0);
	utils::Importer::instance()->deleteGeometryNode(0);
	scene::SceneManager::instance()->deleteGeometryNode(0);

	utils::Importer::instance()->importFile(RESOURCES_PATH "/scenes/dae/house.dae", "house");
	m_renderqueue = scene::SceneManager::instance()->generateRenderQueue();

}
	
void Renderer::setupGL(void)
{
	glClearColor(0.0, 0.0, 0.0, 0.0);
	glPatchParameteri(GL_PATCH_VERTICES, 3);
	glEnable(GL_CULL_FACE);
}

void Renderer::setupShaderStages()
{
	//--- FRAMEBUFFER OBJECTS ------------------------------------------------------------------------------------
	gBuffer = new SlimFBO(size.x, size.y, 2, true, GL_LINEAR);
	sunlightFBO0 = new SlimFBO(size.x / 4, size.y / 4, 1, false, GL_LINEAR);
	sunlightFBO1 = new SlimFBO(size.x / 4, size.y / 4, 1, false, GL_LINEAR);
	sunlightFBO2 = new SlimFBO(size.x / 4, size.y / 4, 1, false, GL_LINEAR);
	sunlightFBO3 = new SlimFBO(size.x / 4, size.y / 4, 1, false, GL_LINEAR);
	sunlightFBO4 = new SlimFBO(size.x / 4, size.y / 4, 1, false, GL_LINEAR);
	compositingFBO = new SlimFBO(size.x, size.y, 1, false, GL_LINEAR);
	downsampledExtractionFBO = new SlimFBO(size.x / 4, size.y / 4, 1, false, GL_LINEAR);

	fsq = new SlimQuad();

	//--- RENDER PASSES ------------------------------------------------------------------------------------------
	blurPass = new SeparatedBlurPass(fsq, size.x / 4, size.y / 4);
	blurPass->inputFBOs.push_back(gBuffer);

	maskPass = new RadialGlowMaskPass(fsq, size.x / 4, size.y / 4);
	maskPass->outputFBO = sunlightFBO3;
	maskPass->inputFBOs.push_back(sunlightFBO2);

	luminancePass = new RadialLuminancePass(fsq, size.x / 4, size.y / 4);
	luminancePass->outputFBO = sunlightFBO4;
	luminancePass->inputFBOs.push_back(sunlightFBO3);

	compositingPass = new CompositingPass(fsq, size.x, size.y);
	compositingPass->outputFBO = compositingFBO;
	compositingPass->inputFBOs.push_back(gBuffer);
	compositingPass->inputFBOs.push_back(sunlightFBO4);

	slowExtractionPass = new CPUExtractionPass(fsq, size.x, size.y);
	slowExtractionPass->inputFBOs.push_back(compositingFBO);
	slowExtractionPass->inputFBOs.push_back(downsampledExtractionFBO);

	fastExtractionPass = new MipMapExtractionPass(fsq, size.x, size.y);
	fastExtractionPass->inputFBOs.push_back(compositingFBO);

	extractionPass = fastExtractionPass;

	finalPass = new FinalPass(fsq, size.x, size.y);
	finalPass->outputFBO = lightingFBO;
	finalPass->inputFBOs.push_back(gBuffer);
	finalPass->inputFBOs.push_back(sunlightFBO3);
	finalPass->inputFBOs.push_back(sunlightFBO4);
	finalPass->inputFBOs.push_back(compositingFBO);

	// Setup sun
	skyScale = glm::mat4(10);
	skyScale[3][3] = 1;
	scene::Transform trans = scene::Transform(glm::vec3(0), glm::toQuat(glm::mat4(1)), glm::vec3(1));
	GLint sunTex = scene::SceneManager::instance()->loadTexture(RESOURCES_PATH "/textures/niceSun.tga", true);

	sun = new scene::Sun(1337, "sun", trans, glm::vec3(1), 1, 1000, 16, sunTex);
	sun->setupShadowMapping(glm::vec2(4096));

	//--- SHADER PROGRAMS ------------------------------------------------------------------------------------
	m_shaderProgram_forward = new ShaderProgram(GLSL::VERTEX, RESOURCES_PATH "/shader/forward/forward.vs.glsl",
												GLSL::FRAGMENT, RESOURCES_PATH "/shader/forward/forward.fs.glsl");
	m_shaderProgram_ocean = new ShaderProgram(GLSL::VERTEX, RESOURCES_PATH "/shader/forward/forward.vs.glsl",
												GLSL::FRAGMENT, RESOURCES_PATH "/shader/forward/forward.fs.glsl",
												GLSL::TESS_CONTROL, RESOURCES_PATH "/shader/forward/forward.tesctrl.glsl",
												GLSL::TESS_EVALUATION, RESOURCES_PATH "/shader/forward/forward.teseval.glsl");
	m_shaderProgram_sky = new ShaderProgram(GLSL::VERTEX, RESOURCES_PATH "/shader/forward/forward.vs.glsl",
											GLSL::FRAGMENT, RESOURCES_PATH "/shader/forward/sky.fs.glsl");
	m_shaderProgram_sun = new ShaderProgram(GLSL::VERTEX, RESOURCES_PATH "/shader/forward/sun.vs.glsl",
											GLSL::FRAGMENT, RESOURCES_PATH "/shader/forward/sun.fs.glsl");
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
}

void Renderer::setupRenderer(GLFWwindow* window)
{
	glm::vec4 wsSunPos = glm::vec4(0, 10, 2000, 1);
	night_tex = scene::SceneManager::instance()->loadTexture(RESOURCES_PATH "/textures/sky/night.dds", true);
	day_tex = scene::SceneManager::instance()->loadTexture(RESOURCES_PATH "/textures/sky/day.dds", true);

	ocean_tex_diffuse = scene::SceneManager::instance()->loadTexture(RESOURCES_PATH "/textures/ocean/water_diffuse.dds", true);
	ocean_tex_specular = scene::SceneManager::instance()->loadTexture(RESOURCES_PATH "/textures/ocean/water-specular.dds", true);
	ocean_tex_normal = scene::SceneManager::instance()->loadTexture(RESOURCES_PATH "/textures/ocean/water_normal.dds", true);

	translucent_tex = scene::SceneManager::instance()->loadTexture(RESOURCES_PATH "/textures/house/statue_translucent.png", true);

	//--- CAMERA --------------------------------------------------------------------------------------------------
	m_scene_camera = new scene::Camera( 0, "scene_camera",
										glm::vec3(30.0f, 10.0f, 10.0f),
										glm::vec3(0.0f, 10.0f, 10.0f),
										glm::vec3(0.0f, 1.0f, 0.0f),
										glm::ivec2(size.x, size.y));
	glm::vec3 camera_position = glm::vec3(1.0f);
	m_scene_camera->SetProjection(scroll, (float)size.x / (float)size.y, 0.1, 3000.0);

	//--- KEYBOARD & MOUSE ----------------------------------------------------------------------------------------
	glfwSetScrollCallback(window, ScrollCallback);
	glfwSetKeyCallback(window, KeyboardCallback);

	//--- UNIFORM SETUP -------------------------------------------------------------------------------------------
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
	forward_uniform_loc_shadow_amount = m_shaderProgram_forward->getUniform("shadow_amount");
	forward_uniform_loc_mouse = m_shaderProgram_forward->getUniform("mouse");
	forward_uniform_loc_shadowMap = m_shaderProgram_forward->getUniform("shadow_map");
	forward_uniform_loc_shadowModel = m_shaderProgram_forward->getUniform("light_model");
	forward_uniform_loc_shadowView = m_shaderProgram_forward->getUniform("light_view");
	forward_uniform_loc_shadowProjection = m_shaderProgram_forward->getUniform("light_projection");
	forward_uniform_loc_translucent_tex = m_shaderProgram_forward->getUniform("translucent_map");
	forward_uniform_loc_translucency = m_shaderProgram_forward->getUniform("translucency");

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

	//! Ocean shader
	use_tesselation_uniform = m_shaderProgram_forward->getUniform("tesselation");
}
	

void Renderer::renderloop(GLFWwindow *window)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	//--- INPUT HANDLING -------------------------------------------------------------------------------------		
	//! Simple camera movement
	old_x = mouse_x;
	old_y = mouse_y;

	glfwGetCursorPos(window, &mouse_x, &mouse_y);

	mouse_correct_x = mouse_x - old_x;
	mouse_correct_y = mouse_y - old_y;

	//mouse_correct_x = ((mouse_x / 1280) * 2.0f) - 1.0f;
	//mouse_correct_y = ((mouse_y / 720) * 2.0f) - 1.0f;


	if (glfwGetMouseButton(glfwindow, GLFW_MOUSE_BUTTON_2))
	{
		m_scene_camera->Rotate(mouse_correct_x * camera_speed, mouse_correct_y * camera_speed);
	}
	if (glfwGetKey(glfwindow, GLFW_KEY_W))
	{
		m_scene_camera->MoveZ(move_speed);
	}
	if (glfwGetKey(glfwindow, GLFW_KEY_S))
	{
		m_scene_camera->MoveZ(-move_speed);
	}
	if (glfwGetKey(glfwindow, GLFW_KEY_D))
	{
		m_scene_camera->MoveX(move_speed);
	}
	if (glfwGetKey(glfwindow, GLFW_KEY_A))
	{
		m_scene_camera->MoveX(-move_speed);
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


	//! ### RENDER SHADOW MAP ############################################   
	sun->update(projection, view);
	if (sun->bloomAmount > 0)
		sun->generateShadowMap(&m_renderqueue);
	
	glViewport(0, 0, size.x, size.y);
	
	//! First shader program:
	//! ### GEOMETRY RENDER ############################################   
	gBuffer->write();
	double forwardTime1 = glfwGetTime();

	glPatchParameteri(GL_PATCH_VERTICES, 3);

	// Set uniforms
	m_shaderProgram_sky->use();
	m_shaderProgram_sky->setUniform(sky_uniform_loc_view, view);
	m_shaderProgram_sky->setUniform(sky_uniform_loc_model, skyScale);
	m_shaderProgram_sky->setUniform(sky_uniform_loc_projection, projection);
	m_shaderProgram_sky->setUniformSampler(sky_uniform_loc_day_tex, day_tex, 1);
	m_shaderProgram_sky->setUniformSampler(sky_uniform_loc_night_tex, night_tex, 2);
	m_shaderProgram_sky->setUniform(sky_uniform_loc_blend, sun->textureBlend);
	m_shaderProgram_sky->setUniform(sky_uniform_loc_color, sun->getColor()*(sun->ambientAmount));
	skyNode->drawTriangles();

	// Draw sun
	m_shaderProgram_sun->use();
	m_shaderProgram_sun->setUniform(sun_uniform_loc_view, view);
	m_shaderProgram_sun->setUniform(sun_uniform_loc_model, sun->getTransform()->getModelMatrix());
	m_shaderProgram_sun->setUniform(sun_uniform_loc_projection, projection);
	m_shaderProgram_sun->setUniformSampler(sun_uniform_loc_tex, sun->sunTexture, 1);
	m_shaderProgram_sun->setUniform(sun_uniform_loc_color, sun->getColor());
	glDisable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glDepthMask(GL_FALSE);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	sun->render();
	glEnable(GL_DEPTH_TEST);
	glDepthMask(GL_TRUE);
	glDisable(GL_BLEND);


	
	//glEnable(GL_BLEND);
	//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	// Forward rendering
	m_shaderProgram_forward->use();
	m_shaderProgram_forward->setUniform(use_tesselation_uniform, FALSE);
	m_shaderProgram_forward->setUniform(forward_uniform_loc_mouse, glm::vec2(mouse_correct_x, mouse_correct_y));
	m_shaderProgram_forward->setUniform(forward_uniform_loc_view, view);
	m_shaderProgram_forward->setUniform(forward_uniform_loc_projection, projection);
	m_shaderProgram_forward->setUniform(forward_uniform_loc_light_position, sun->getTransform()->getPosition());
	m_shaderProgram_forward->setUniform(forward_uniform_loc_light_color, sun->getColor());
	m_shaderProgram_forward->setUniform(forward_uniform_loc_ambient_amount, sun->ambientAmount);
	m_shaderProgram_forward->setUniform(forward_uniform_loc_diffuse_amount, sun->diffuseAmount);
	m_shaderProgram_forward->setUniform(forward_uniform_loc_shadow_amount, sun->bloomAmount);

	m_shaderProgram_forward->setUniform(forward_uniform_loc_ambient_amount, sun->ambientAmount);
	m_shaderProgram_forward->setUniform(forward_uniform_loc_diffuse_amount, sun->diffuseAmount);

	m_shaderProgram_forward->setUniformSampler(forward_uniform_loc_shadowMap, sun->getShadowMap(), 3);
	//m_shaderProgram_forward->setUniform(forward_uniform_loc_shadowModel, );
	m_shaderProgram_forward->setUniform(forward_uniform_loc_shadowView, sun->getViewMatrix());
	m_shaderProgram_forward->setUniform(forward_uniform_loc_shadowProjection, sun->getProjectionMatrix());

	//render geometry nodes 
	for (unsigned int i = 0; i < m_renderqueue.size(); i++)
	{
		m_shaderProgram_forward->setUniform(forward_uniform_loc_model, m_renderqueue[i]->getTransform()->getModelMatrix());
		m_shaderProgram_forward->setUniform(forward_uniform_loc_diffuse_color, *(m_renderqueue[i]->getMaterial()->getDiffuseColor()));
		m_shaderProgram_forward->setUniform(forward_uniform_loc_specular_color, *(m_renderqueue[i]->getMaterial()->getSpecularColor()));
		m_shaderProgram_forward->setUniform(forward_uniform_loc_shininess, m_renderqueue[i]->getMaterial()->getShininess());
		m_shaderProgram_forward->setUniform(forward_uniform_loc_light_color, sun->getColor());
		m_shaderProgram_forward->setUniformSampler(forward_uniform_loc_diffuse_tex, m_renderqueue[i]->getMaterial()->getDiffuseTexture(), 0);
		m_shaderProgram_forward->setUniformSampler(forward_uniform_loc_specular_tex, m_renderqueue[i]->getMaterial()->getSpecularTexture(), 1);
		m_shaderProgram_forward->setUniformSampler(forward_uniform_loc_normal_tex, m_renderqueue[i]->getMaterial()->getNormalTexture(), 2);
		m_shaderProgram_forward->setUniformSampler(forward_uniform_loc_translucent_tex, translucent_tex, 4);
		m_shaderProgram_forward->setUniform(forward_uniform_loc_translucency, m_renderqueue[i]->getMaterial()->getTranslucency());

		m_renderqueue[i]->drawTriangles();
	}

	//! Draw Ocean
	//m_shaderProgram_ocean->use();
	m_shaderProgram_forward->setUniform(use_tesselation_uniform, FALSE);
	m_shaderProgram_forward->setUniform(forward_uniform_loc_model, oceanNode->getTransform()->getModelMatrix() * skyScale);
	m_shaderProgram_forward->setUniform(forward_uniform_loc_diffuse_color, glm::vec3(0.0, 0.3, 1.0));
	m_shaderProgram_forward->setUniform(forward_uniform_loc_specular_color, glm::vec3(1.0));
	m_shaderProgram_forward->setUniform(forward_uniform_loc_shininess, 200.0f);
	m_shaderProgram_forward->setUniformSampler(forward_uniform_loc_diffuse_tex, ocean_tex_diffuse, 3);
	m_shaderProgram_forward->setUniformSampler(forward_uniform_loc_specular_tex, ocean_tex_specular, 4);
	m_shaderProgram_forward->setUniformSampler(forward_uniform_loc_normal_tex, ocean_tex_normal, 5);
	oceanNode->drawTriangles();
	//m_shaderProgram_ocean->unuse();

	m_shaderProgram_forward->unuse();
	gBuffer->unbind();
	glDisable(GL_BLEND);

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

	//! Clear all used shader programs
	m_framecount++;
	glUseProgram(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
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

