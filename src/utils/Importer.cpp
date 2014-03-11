//! @file Importer.cpp
#include "Importer.h"

namespace utils {
	
	Importer::Importer(void)
	{
		m_aiScene = 0;
		FreeImage_Initialise(false);
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
            std::cerr << m_aiImporter.GetErrorString() << std::endl;
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
            //! Helper array to print light types to string
            const char* light_type_str[] = {
                "UNDEFINED",
                "DIRECTIONAL",
                "POINT",
                "SPOT"
            };

            scene::Light* new_light;
			//! Process lights
			for (unsigned int light_id = 0; light_id < m_aiScene->mNumLights; light_id++)
			{
				aiLight* current_light = m_aiScene->mLights[light_id];
					
				//! Name
				aiString name = current_light->mName;

				//! Position & orientation
                //! \todo blender currently has problems exporting the lights position
                aiVector3D position         = current_light->mPosition;
                position.x                  =   0.0;
                position.y                  =   4.0;
                position.z                  = -15.0;
                aiVector3D direction        = current_light->mDirection;
                aiLightSourceType lighttype = current_light->mType;
                aiColor3D color             = current_light->mColorAmbient;

                new_light = new scene::Light(light_id,
                                             name.C_Str(),
                                             scene::Transform( glm::vec3(position.x, position.y, position.z),
                                                               glm::quat(1.0f, glm::vec3(direction.x, direction.y, direction.z) ),
                                                               glm::vec3(1.0) ),
                                             glm::vec3(color.r, color.g, color.b), 1.0f);

                //! Add to scene manager
                scene::SceneManager::instance()->addLight(new_light);

                //! Log
				std::cout << "\n  * Light: " << light_id << std::endl;
				std::cout << "    Name: " << name.C_Str() << std::endl;
                std::cout << "    Type: " << light_type_str[lighttype] << std::endl;
				std::cout << "    Position  (" << position.x << ", " << position.y << ", " << position.z << ")" << std::endl;
				std::cout << "    Direction (" << direction.x << ", " << direction.y << ", " << direction.z << ")" << std::endl;
                std::cout << "    Color:    (" << color.r << ", " << color.g << ", " << color.b << ")" << std::endl;
			}
		}

		//! ------ Meshes ------------------------------------------
		if (m_aiScene->HasMeshes())
		{
            std::cout << "Number meshes in Root Node: " << m_aiScene->mRootNode->mNumMeshes << std::endl;
            std::cout << "Number meshes in m_aiScene: " << m_aiScene->mNumMeshes << std::endl;

            //! Get root transformtaion matrix
            aiMatrix4x4 root_transform = m_aiScene->mRootNode->mTransformation;

            scene::Geometry* new_geometry;
            //! Process meshes
            for (unsigned int mesh_id = 0; mesh_id < m_aiScene->mNumMeshes; mesh_id++)
            {
                //! Get current node from assimp scene
                aiNode* current_node = m_aiScene->mRootNode->mChildren[mesh_id];

                aiString name = current_node->mName;

                //! Transformation
                aiVector3D current_position, current_scale;
                aiQuaternion current_rotation;
                aiMatrix4x4 m_transform = current_node->mTransformation;
                //! The mesh need to be transformed with a general scene transformation matrix first
                m_transform = root_transform * m_transform;
                m_transform.Decompose(current_scale, current_rotation, current_position);

                //! Create new geometry node and set its transform
                new_geometry = new scene::Geometry(mesh_id, name.C_Str());
                new_geometry->setTransform(
                    scene::Transform(glm::vec3(current_position.x, current_position.y, current_position.z),
                    glm::quat(current_rotation.w, glm::vec3(current_rotation.x, current_rotation.y, current_rotation.z)),
                    glm::vec3(current_scale.x, current_scale.y, current_scale.z) )
                );

                aiMesh* current_mesh = m_aiScene->mMeshes[mesh_id];
                unsigned int material_index  = current_mesh->mMaterialIndex;
                new_geometry->setMaterialIndex(material_index);

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
                std::cout << "    Material index: " << material_index << std::endl;
            }
        }

		//! ------ Materials ------------------------------------------
		if (m_aiScene->HasMaterials())
		{
            scene::Material* new_material;

            //! Read materials and textures and organize them: HashMap, Smart-Pointers
            for(unsigned int material_id = 0; material_id < m_aiScene->mNumMaterials; material_id++)
            {
                aiMaterial* current_material = m_aiScene->mMaterials[material_id];


                aiString name;
                current_material->Get(AI_MATKEY_NAME, name);

                aiColor3D diffuse;
                current_material->Get(AI_MATKEY_COLOR_DIFFUSE, diffuse);

                aiColor3D specular;
                current_material->Get(AI_MATKEY_COLOR_SPECULAR, specular);

                float shininess;
                current_material->Get(AI_MATKEY_SHININESS, shininess);


                //! --- Textures ---
                //! Diffuse
                aiString texture_path_diffuse;
                current_material->GetTexture(aiTextureType_DIFFUSE, 0, &texture_path_diffuse);
                //! Specular
                aiString texture_path_specular;
                current_material->GetTexture(aiTextureType_SPECULAR, 0, &texture_path_specular);
                //! Normal
                aiString texture_path_normal;
                current_material->GetTexture(aiTextureType_NORMALS, 0, &texture_path_normal);

                //! Create new material
                new_material = new scene::Material(material_id, // Index-number
                                                   glm::vec3(diffuse.r, // Diffuse Color
                                                             diffuse.g,
                                                             diffuse.b),
                                                   scene::SceneManager::instance()->loadTexture(texture_path_diffuse.C_Str(),true),
                                                   glm::vec3(specular.r,
                                                             specular.g,
                                                             specular.b),
                                                   scene::SceneManager::instance()->loadTexture(texture_path_specular.C_Str(),true),
                                                   shininess,
                                                   scene::SceneManager::instance()->loadTexture(texture_path_normal.C_Str(),true));

                //! Add material to scene manager
                scene::SceneManager::instance()->addMaterial(new_material);

                //! Log
                std::cout << "\n  * Material: " << material_id << std::endl;
                std::cout << "      " << name.C_Str() << std::endl;
                std::cout << "      Diffuse-Color:   (" << diffuse.r << ", " << diffuse.g << ", " << diffuse.b << ")" << std::endl;
                std::cout << "      Diffuse-Texture: (" << texture_path_diffuse.C_Str() << ")" << std::endl;
                std::cout << "      Specular-Color:  (" << specular.r << ", " << specular.g << ", " << specular.b << ")" << std::endl;
                std::cout << "      Specular-Texture:(" << texture_path_specular.C_Str() << ")" << std::endl;
                std::cout << "      Shininess:        " << shininess << std::endl;
                std::cout << "      Normal-Texture:  (" << texture_path_normal.C_Str() << ")" << std::endl;
            }
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

	void Importer::deleteGeometryNode(const unsigned int index)
	{
		if (index > m_geometry_node_list.size())
		{
			std::cerr << "ERROR (Importer): wrong scene node index" << std::endl;
			return;
		}
		else
		{
			m_geometry_node_list.erase(m_geometry_node_list.begin()+index);
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
}
