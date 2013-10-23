//! @file Renderer.cpp
#include "Renderer.h"
 
namespace tone {
        namespace renderer {
                //! \brief Constructor
                Renderer::Renderer(void)
                {
                        //! Initialize fps variables
                        m_framecount = 0;
                        m_fps = 0;
                        m_current_time = 0.0;
                        m_previous_time = 0.0;
 
                        init();
                }
 
                //! \brief Destructor
                Renderer::~Renderer(void)
                {
                }
 
                //! \brief Returns singleton instance
                Renderer* Renderer::instance(void)
                {
					static Renderer m_instance;
					return &m_instance;
                }

				//! \brief Sets the context to render to
				void Renderer::setRenderContext(Context& context)
				{
					m_context = &context;
				}
 
				//! \brief Initializes the renderer
                void Renderer::init()
                {	
					setupGL();
                }

				//! \brief Sets all the initial OpenGL flags
				void Renderer::setupGL(void)
				{
                    //! OpenGL settings
                    glClearColor(0.0, 0.0, 0.0, 1.0);
                    glEnable(GL_DEPTH_TEST);
				}

				/*
                //! \brief Handles mouse events
                void Renderer::mouseCallback_implementation(int button, int action)
				{
					if( !TwEventMouseButtonGLFW(button, action) )  // send event to AntTweakBar
					{
						glm::vec4 viewport = glm::vec4(0, 0, m_context->getSize().x, m_context->getSize().y);
						glm::vec3 cursor_coords = glm::vec3(m_mouse_pos.x, m_context->getSize().y - m_mouse_pos.y - 1, 1.00);
						glm::vec3 new_object_coords = glm::unProject(cursor_coords, m_main_camera->GetViewMatrix(), m_main_camera->GetProjectionMatrix(), viewport);
						glm::vec3 new_object_coords_vec = glm::normalize(new_object_coords);

						glm::vec3 new_object_position = m_main_camera->GetPosition() + glm::vec3(20.0) * new_object_coords_vec;

						if( button == GLFW_MOUSE_BUTTON_1 && action == GLFW_RELEASE )
						{
							utils::log() << "MOUSE 1 was presses!\n";
							switch(gui_body_selection)
							{
								case DYNAMIC:
								{
									if(m_sceneNode_counter > m_importer->m_sceneNode_list.size()-1)
										m_sceneNode_counter = 0;

									glm::quat newRot = glm::quat(1.0, glm::vec3(0.0));
									glm::normalize(newRot);
									glm::vec3 veloc_dir = (new_object_coords - m_main_camera->GetPosition()) * gui_push_force;

									scene::Geometry* new_geo = dynamic_cast<scene::Geometry*>( m_importer->m_sceneNode_list.at(m_sceneNode_counter) );

									glm::vec3 pos =  glm::vec3(0.0);
									pos.x += rand()%10;
									pos.y += rand()%10; 
									pos.z += rand()%10;

									if(abs(glfwGetTime() - m_lastcreated_time) > 0.25)
									{
										system->createBody(*new_geo, m_main_camera->GetPosition(), newRot, 1.0, veloc_dir, glm::vec3(3.0));
										m_lastcreated_time = glfwGetTime();
									}
									
									m_sceneNode_counter++;

									break;
								}
								case STATIC:
									break;
								case PARTICLE_SYSTEM:
								{
									glm::vec3 particle_system_origin = new_object_position;
									m_particleSystem = std::make_shared<physics::ParticleSystem>(particle_system_origin, -1, gui_particleCount, 0.001);
									break;
								}
							}
						}
						if( button == GLFW_MOUSE_BUTTON_MIDDLE && action == GLFW_RELEASE )
						{
							utils::log() << "MOUSE MIDDLE was presses!\n";
							glm::vec3 particle_system_origin = m_main_camera->GetLookAt();
							m_particleSystem = std::make_shared<physics::ParticleSystem>(particle_system_origin, 0.5, gui_particleCount, 0.001);
						}
					} 
                }
 
                //! \brief Handles keyboard events
                void Renderer::keyboardCallback_implementation(int key, int action)
                {
					if(!(TwEventKeyGLFW(key, action)))
					{
						if( key == GLFW_KEY_X && action == GLFW_RELEASE )
						{
							testparticle = 0;
						}
						if( key == GLFW_KEY_0 && action == GLFW_RELEASE )
						{						
							system->togglePause();
						}
					    if( key == GLFW_KEY_R && action == GLFW_RELEASE )
						{
							resetPyramid();
						}
					}
                }

				void Renderer::cursorCallback_implementation(double mouseX, double mouseY)
				{
					if( !TwEventMousePosGLFW(mouseX, mouseY) )  // send event to AntTweakBar
					{ 
		
					}
				}
				*/

                //! \brief Render loop
                void Renderer::renderloop()
                {
					while (m_context && m_context->isLive())
					{
						glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

						m_context->swapBuffers();
						m_framecount++;
					}
                }
        }
}