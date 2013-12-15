//! @file Importer.cpp
#include "Importer.h"

namespace utils {
	
	Importer::Importer(void)
	{
		m_aiScene = 0;
	}

	Importer::~Importer(void)
	{
	}

	Importer* Importer::instance(void)
	{
		static Importer m_importer;
		return &m_importer;
	}

	void Importer::importFile(const std::string& pathToFile)
	{
		m_aiScene = m_aiImporter.ReadFile(pathToFile,  
			aiProcess_Triangulate | aiProcess_JoinIdenticalVertices | aiProcess_RemoveRedundantMaterials);
			
		if(!m_aiScene)
		{
			std::cerr << "ERROR (Importer): Scene was not successfully loaded!" << std::endl;
		}
		else
		{
			//! Do the scene processing
			processScene();
		}
	}

	void Importer::processScene(void)
	{
		aiString root_name = m_aiScene->mRootNode->mName;

		//! Log
		std::cout << "\n--- IMPORTER -------------------------------------------------------------" << std::endl;
		std::cout << "Root node name: " << root_name.C_Str() << std::endl;
		std::cout << "Mesh count: " << m_aiScene->mNumMeshes << std::endl;
		std::cout << "Material count: " << m_aiScene->mNumMaterials << std::endl;
		std::cout << "Light count: " << m_aiScene->mNumLights << std::endl;
		std::cout << "Camera count: " << m_aiScene->mNumCameras << std::endl;
		std::cout << "\nList of Nodes:" << std::endl;

		//! ------ Cameras ------------------------------------------  
		if (m_aiScene->HasCameras())
		{
			scene::Camera* new_camera;
			//! Process cameras
			for (unsigned int camera_id = 0; camera_id < m_aiScene->mNumCameras; camera_id++)
			{	
				aiCamera* current_camera = m_aiScene->mCameras[camera_id];

				//! Name
				aiString name = current_camera->mName;
					
				//! Position & orientation
				aiVector3D position	= current_camera->mPosition;
				aiVector3D lookat	= current_camera->mLookAt;
				aiVector3D up		= current_camera->mUp;

				//! Projection
				float field_of_view	= current_camera->mHorizontalFOV;
				float aspect		= current_camera->mAspect;
				float near_plane	= current_camera->mClipPlaneNear;
				float far_plane		= current_camera->mClipPlaneFar;
			
				//! Create a camera
				new_camera = new scene::Camera(camera_id, name.C_Str(),
							       glm::vec3(position.x, position.y, position.z),
							       glm::vec3(lookat.x, lookat.y, lookat.z),
							       glm::vec3(up.x, up.y, up.z),
							       glm::ivec2(1024, 1024));
				new_camera->SetProjection(field_of_view, aspect, near_plane, far_plane);

				//! Add to scene manager
				scene::SceneManager::instance()->addSceneNode(new_camera);

				//! Log
				std::cout << "\n  * Camera: " << camera_id << std::endl;
				std::cout << "    Name: " << name.C_Str() << std::endl;
				std::cout << "    Position (" << position.x << ", " << position.y << ", " << position.z << ")" << std::endl;
				std::cout << "    Look At (" << lookat.x << ", " << lookat.y << ", " << lookat.z << ")" << std::endl;
				std::cout << "    Up (" << up.x << ", " << up.y << ", " << up.z << ")" << std::endl;
				std::cout << "    Aspect:" << aspect << std::endl;
				std::cout << "    FOV: " << field_of_view << std::endl;
				std::cout << "    Frustum, Near:" << near_plane << "Far: " << far_plane << std::endl;
			}
		}

		//! ------ Lights ------------------------------------------
		if (m_aiScene->HasLights())
		{
			//! Process lights
			for (unsigned int light_id = 0; light_id < m_aiScene->mNumLights; light_id++)
			{
				aiLight* current_light = m_aiScene->mLights[light_id];
					
				//! Name
				aiString name = current_light->mName;

				//! Position & orientation
				aiVector3D position  = current_light->mPosition;
				aiVector3D direction = current_light->mDirection;

				//! Colors
				aiColor3D ambient  = current_light->mColorAmbient;
				aiColor3D diffuse  = current_light->mColorDiffuse;
				aiColor3D specular = current_light->mColorSpecular;

				//! Log
				std::cout << "\n  * Light: " << light_id << std::endl;
				std::cout << "    Name: " << name.C_Str() << std::endl;
				std::cout << "    Position  (" << position.x << ", " << position.y << ", " << position.z << ")" << std::endl;
				std::cout << "    Direction (" << direction.x << ", " << direction.y << ", " << direction.z << ")" << std::endl;
				std::cout << "    Color Ambient:  (" << ambient.r << ", " << ambient.g << ", " << ambient.b << ")" << std::endl;
				std::cout << "    Color Diffuse:  (" << diffuse.r << ", " << diffuse.g << ", " << diffuse.b << ")" << std::endl;
				std::cout << "    Color Specular: (" << specular.r << ", " << specular.g << ", " << specular.b << ")" << std::endl;
			}
		}

		//! ------ Meshes ------------------------------------------
		if (m_aiScene->HasMeshes())
		{
			//! Traverse all scene nodes to get transformations of meshes
			for (unsigned int node = 0; node < m_aiScene->mRootNode->mNumChildren; node++)
			{
				scene::Geometry* new_geometry;
				//! Process meshes
				for (unsigned int mesh_id = 0; mesh_id < m_aiScene->mNumMeshes; mesh_id++)
				{
					//! Get current node from assimp scene
					aiNode* current_node = m_aiScene->mRootNode->mChildren[node];

					aiString name = current_node->mName;

					//! Transformation
					aiVector3D current_position, current_scale;
					aiQuaternion current_rotation;
					current_node->mTransformation.Decompose(current_scale, current_rotation, current_position);

					//! Create new geometry node and set its transform
					new_geometry = new scene::Geometry(mesh_id, name.C_Str());
					new_geometry->setTransform(
						scene::Transform(glm::vec3(current_position.x, current_position.y, current_position.z),
						glm::quat(current_rotation.w, glm::vec3(current_rotation.x, current_rotation.y, current_rotation.z)),
						glm::vec3(current_scale.x, current_scale.y, current_scale.z) )
					);

					aiMesh* current_mesh = m_aiScene->mMeshes[mesh_id];
					//! Geometry
					if (current_mesh->HasPositions())
					{
						//! Vertices
						for (unsigned int vertex = 0; vertex <= current_mesh->mNumVertices; vertex++)
						{
							aiVector3D* current_vertex = &(current_mesh->mVertices[vertex]);
							new_geometry->addVertex(current_vertex->x, current_vertex->y, current_vertex->z);

							//! Normals
							if (current_mesh->HasNormals())
							{
								aiVector3D* current_normal = &(current_mesh->mNormals[vertex]);
								new_geometry->addNormal(current_normal->x, current_normal->y, current_normal->z);
							}

							//! Texture coordinates
							if (current_mesh->HasTextureCoords(0))
							{
								aiVector3D* current_uv = &(current_mesh->mTextureCoords[0][vertex]);
								new_geometry->addUV(current_uv->x, current_uv->y);
							}
						}
					}
					//! Faces (indices for vertex list)
					if (current_mesh->HasFaces())
					{
						for (unsigned int face = 0; face < current_mesh->mNumFaces; face++)
						{
							aiFace* current_face = &(current_mesh->mFaces[face]);

							//! All three indices of the current triangle face
							int vertex_index0 = current_mesh->mFaces[face].mIndices[0];
							int vertex_index1 = current_mesh->mFaces[face].mIndices[1];
							int vertex_index2 = current_mesh->mFaces[face].mIndices[2];

							new_geometry->addIndex(vertex_index0);
							new_geometry->addIndex(vertex_index1);
							new_geometry->addIndex(vertex_index2);
						}
					}

					new_geometry->createBuffers();

					//! Add to scene manager
					m_geometry_node_list.push_back(new_geometry);
					scene::SceneManager::instance()->addSceneNode(new_geometry);

					//! Log
					std::cout << "\n  * Mesh: " << mesh_id << std::endl;
					std::cout << "    Name: " << name.C_Str() << std::endl;
					std::cout << "    Vertex count: " << current_mesh->mNumVertices << std::endl;
					std::cout << "    Faces count: " << current_mesh->mNumFaces << std::endl;
					std::cout << "    Normals count: " << current_mesh->mNumVertices << std::endl;
				}
			}
		}

		//! ------ Materials ------------------------------------------
		if (m_aiScene->HasMaterials())
		{
			//! \todo Read materials and textures and organize them: HashMap, Smart-Pointers
		}
	}

	scene::Geometry* Importer::getGeometryNode(const unsigned int index)
	{
		if (index > m_geometry_node_list.size())
		{
			std::cerr << "ERROR (Importer): wrong scene node index" << std::endl;
			return 0;
		}
		else
		{
			return m_geometry_node_list[index];
		}
	}


	scene::Camera* Importer::getCameraNode(const unsigned int index)
	{
		if (index > m_geometry_node_list.size())
		{
			std::cerr << "ERROR (Importer): wrong scene node index" << std::endl;
			return 0;
		}
		else
		{
			return m_camera_node_list[index];
		}
	}

	GLuint Importer::loadTexture(std::string filename)
	{
		GLuint tex_2d;
			
		glActiveTexture(GL_TEXTURE0);
		glGenTextures( 1, &tex_2d );
		glBindTexture( GL_TEXTURE_2D, tex_2d );

		tex_2d = SOIL_load_OGL_texture
		(
			filename.c_str(),
			SOIL_LOAD_AUTO,
			SOIL_CREATE_NEW_ID,
			SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
		);

		return tex_2d;
	}
}
