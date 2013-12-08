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
				new_camera = new scene::Camera(name.C_Str(),
							       glm::vec3(position.x, position.y, position.z),
							       glm::vec3(lookat.x, lookat.y, lookat.z),
							       glm::vec3(up.x, up.y, up.z),
							       glm::ivec2(1024, 1024));
				new_camera->SetProjection(field_of_view, aspect, near_plane, far_plane);

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
			scene::Geometry* new_geometry;
			//! Process meshes
			for (unsigned int mesh_id = 0; mesh_id < m_aiScene->mNumMeshes; mesh_id++)
			{
				aiMesh* current_mesh = m_aiScene->mMeshes[mesh_id];

				//! Name
				aiString name = current_mesh->mName;

				//! Geometry
				if (current_mesh->HasPositions())
				{
					//! Vertices
					for (unsigned int vertex = 0; vertex <= current_mesh->mNumVertices; vertex++)
					{
						aiVector3D* current_vertex = &(current_mesh->mVertices[vertex]);
				
						//! Normals
						if (current_mesh->HasNormals())
						{
							aiVector3D* current_normal = &(current_mesh->mNormals[vertex]);
						}
				
						//! Texture coordinates
						if (current_mesh->HasTextureCoords(0))
						{
							aiVector3D* current_uv = &(current_mesh->mTextureCoords[0][vertex]);
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
						int x = current_mesh->mFaces[face].mIndices[0];
						int y = current_mesh->mFaces[face].mIndices[1];
						int z = current_mesh->mFaces[face].mIndices[2];
					}
				}

				//! Log
				std::cout << "\n  * Mesh: " << mesh_id << std::endl;
				std::cout << "    Name: " << name.C_Str() << std::endl;
				std::cout << "    Vertex count: " << current_mesh->mNumVertices << std::endl;
				std::cout << "    Faces count: " << current_mesh->mNumFaces << std::endl;
				std::cout << "    Normals count: " << current_mesh->mNumVertices << std::endl;
			}

		}

		//! ------ Materials ------------------------------------------
		if (m_aiScene->HasMaterials())
		{

		}
	}

	void Importer::processGeometry(const unsigned int& mesh_index, scene::Transform* node_transform)
	{
		scene::Geometry* node_geometry = new scene::Geometry(mesh_index);
		node_geometry->setTransform(*node_transform);
		aiMesh* aimesh = m_aiScene->mMeshes[mesh_index];

			std::cout << "REAL POS " << aimesh->mNumVertices << std::endl;
		//! Process Vertices
		for(unsigned int vertex = 0; vertex < aimesh->mNumVertices; vertex++)
		{
			std::cout << "vertex # " << vertex << std::endl;

			//! Create vertex list
			if (aimesh->HasPositions())
			{
				node_geometry->addVertex(aimesh->mVertices[vertex].x,
										 aimesh->mVertices[vertex].y,
										 aimesh->mVertices[vertex].z);
			}

			//! Create normals list
			if(aimesh->HasNormals())
			{
				node_geometry->addNormal(aimesh->mNormals[vertex].x,
										 aimesh->mNormals[vertex].y,
										 aimesh->mNormals[vertex].z);
			}
			//! Create texture coordinate list
			if(aimesh->HasTextureCoords(0))
			{
				node_geometry->addUV(aimesh->mTextureCoords[0][vertex].x,
									 aimesh->mTextureCoords[0][vertex].y);
			}
		}
	
		//! Create index list
		for(unsigned int face = 0; face < aimesh->mNumFaces; face++)
		{
			//! Triangle
			int vertex0_index = aimesh->mFaces[face].mIndices[0];
			int vertex1_index = aimesh->mFaces[face].mIndices[1];
			int vertex2_index = aimesh->mFaces[face].mIndices[2];

			node_geometry->addIndex(vertex0_index);
			node_geometry->addIndex(vertex1_index);
			node_geometry->addIndex(vertex2_index);
		}
		//! Log
		std::cout << "      Geometry Information:\n";
		std::cout << "      - Vertex count:        " << node_geometry->vertexCount() << std::endl;
		std::cout << "      - Index count:         " << node_geometry->indexCount()  << std::endl;
		std::cout << "      - Normal count:        " << node_geometry->normalCount() << std::endl;
		std::cout << "      - UV coordinate count: " << node_geometry->uvCount()     << std::endl;

		node_geometry->createBuffers();
		
		m_sceneNode_list.push_back(node_geometry);
	}

	scene::SceneNode* Importer::getSceneNode(const int index)
	{
		if (index > m_sceneNode_list.size())
		{
			//utils::log() << "ERROR (Importer): wrong scene node index\n";
			return 0;
		}
		else
		{
			return m_sceneNode_list[index];
		}
	}

	void Importer::loadTexture(std::string filename)
	{
		/*
		GLuint tex_2d;
			
		glActiveTexture(GL_TEXTURE0);
		glGenTextures( 1, &tex_2d );
		glBindTexture( GL_TEXTURE_2D, tex_2d );

		tex_2d = SOIL_load_OGL_texture(
			filename.c_str(),
			SOIL_LOAD_AUTO,
			SOIL_CREATE_NEW_ID,
			SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
		);

		if( 0 == tex_2d )
		{
			std::cout << "SOIL loading error:" << SOIL_last_result() << std::endl;
		}
		m_texture_list.push_back(tex_2d);
		*/
	}
}
