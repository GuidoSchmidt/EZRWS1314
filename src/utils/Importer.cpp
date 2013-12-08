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
			aiProcess_Triangulate | 
			aiProcess_CalcTangentSpace |
			aiProcess_JoinIdenticalVertices |
			aiProcess_SortByPType |
			aiProcess_OptimizeMeshes |
			aiProcess_FixInfacingNormals);
			
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

        std::cout << "--- IMPORTER -------------------------------------------------------------" << std::endl;
        std::cout << "Root node name: " << root_name.C_Str() << std::endl;
        std::cout << "  > Mesh count: " << m_aiScene->mNumMeshes << std::endl;
        std::cout << "  > Material count: " << m_aiScene->mNumMaterials << std::endl;
        std::cout << "  > Light count: " << m_aiScene->mNumLights << std::endl;
        std::cout << "  > Camera count: " << m_aiScene->mNumCameras << std::endl;

        std::cout << "\n  > Nodes:" << std::endl;

		for(unsigned int node = 0; node < m_aiScene->mRootNode->mNumChildren; node++)
		{
            std::cout << "    * Node name: ";
			aiString node_name = m_aiScene->mRootNode->mChildren[node]->mName;
            std::cout << node_name.C_Str() << std::endl;
			if(m_aiScene->mRootNode->mChildren[node]->mNumMeshes != 0)
			{
                std::cout << "Node Type: GEOMETRY" << std::endl;
				unsigned int mesh_id = *(m_aiScene->mRootNode->mChildren[node]->mMeshes);

				//! Get nodes transformation
				aiVector3D aiPosition, aiScale;
				aiQuaternion aiRotation;
				m_aiScene->mRootNode->mChildren[node]->mTransformation.Decompose(aiScale, aiRotation, aiPosition);

				aiVector3D aiPosition_root;
				aiQuaternion aiRotation_root;
				m_aiScene->mRootNode->mTransformation.DecomposeNoScaling(aiRotation_root, aiPosition_root);

				aiQuaternion aiRotation_complete = aiRotation * aiRotation_root;

				glm::vec3 node_translate = glm::vec3(aiPosition.x, aiPosition.y, aiPosition.z);
				glm::quat node_rotate	 = glm::quat(aiRotation.w, aiRotation.x, aiRotation.y, aiRotation.z);
				glm::vec3 node_scale	 = glm::vec3(aiScale.x, aiScale.y, aiScale.z);

				scene::Transform node_transform(node_translate, node_rotate, node_scale);
					
				/*
				utils::log() << "	Transform:\n";
				utils::log() << "	- Translation: ";
				utils::log() <<	node_translate;
				utils::log() << "\n";
				utils::log() << "	- Scale: ";
				utils::log() <<	node_scale;
				utils::log() << "\n";
				utils::log() << "	- Rotation: ";
				utils::log() <<	glm::eulerAngles(node_rotate);
				utils::log() << "\n";
				*/
                processGeometry(mesh_id, &node_transform);
			}
			//utils::log() << "\n\n";
		}
		//utils::log() << "\n\n";
	}

	void Importer::processGeometry(const unsigned int& mesh_index, scene::Transform* node_transform)
	{
		scene::Geometry* node_geometry = new scene::Geometry(mesh_index);
		node_geometry->setTransform(*node_transform);
		aiMesh* aimesh = m_aiScene->mMeshes[mesh_index];

		//! Process Vertices
		for(unsigned int vertex = 0; vertex < aimesh->mNumVertices; vertex++)
		{
			//! Create vertex list
			node_geometry->addVertex(aimesh->mVertices[vertex].x,
										aimesh->mVertices[vertex].y,
										aimesh->mVertices[vertex].z);

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
					
			//! Create face-list for physics and add to geometry
            /*
            std::vector<GLint> indices;
			indices.clear();
			indices.push_back(vertex0_index);
			indices.push_back(vertex1_index);
			indices.push_back(vertex2_index);

			glm::vec3 normal(0.0);
			glm::vec3 a_temp, b_temp;
			glm::vec3 normal_one =  glm::vec3(aimesh->mNormals[vertex0_index].x, aimesh->mNormals[vertex0_index].y, aimesh->mNormals[vertex0_index].z);				
			glm::vec3 normal_two =  glm::vec3(aimesh->mNormals[vertex1_index].x, aimesh->mNormals[vertex1_index].y, aimesh->mNormals[vertex1_index].z);				
			glm::vec3 normal_three =  glm::vec3(aimesh->mNormals[vertex2_index].x, aimesh->mNormals[vertex2_index].y, aimesh->mNormals[vertex2_index].z);
			normal.x = (normal_one.x + normal_two.x + normal_three.x) / 3.0f;
			normal.y = (normal_one.y + normal_two.y + normal_three.y) / 3.0f;
			normal.z = (normal_one.z + normal_two.z + normal_three.z) / 3.0f;
				
			glm::vec3 vertex0 = glm::vec3( aimesh->mVertices[vertex0_index].x, aimesh->mVertices[vertex0_index].y, aimesh->mVertices[vertex0_index].z );
			glm::vec3 vertex1 = glm::vec3( aimesh->mVertices[vertex1_index].x, aimesh->mVertices[vertex1_index].y, aimesh->mVertices[vertex1_index].z );
			glm::vec3 vertex2 = glm::vec3( aimesh->mVertices[vertex2_index].x, aimesh->mVertices[vertex2_index].y, aimesh->mVertices[vertex2_index].z );

			glm::vec3 centroid = glm::vec3(0);
			centroid.x = 1.0f/3.0f * vertex0.x + 1.0f/3.0f * vertex1.x + 1.0f/3.0f * vertex2.x; 
			centroid.y = 1.0f/3.0f * vertex0.y + 1.0f/3.0f * vertex1.y + 1.0f/3.0f * vertex2.y; 
			centroid.z = 1.0f/3.0f * vertex0.z + 1.0f/3.0f * vertex1.z + 1.0f/3.0f * vertex2.z; 
            */
		}
		//! Log
		/*
		utils::log() << "    Vertex count:                  ";
		utils::log() << node_geometry->vertexCount();
		utils::log() << "\n    Index count:                   ";
		utils::log() << node_geometry->indexCount();
		utils::log() << "\n    Normal count:                  ";
		utils::log() << node_geometry->normalCount();
		utils::log() << "\n    Texture coordinate (UV) count: ";
		utils::log() << node_geometry->uvCount();
		*/

		node_geometry->createBuffers();
        //node_geometry->createNormalsGeometry();
        //node_geometry->createBoundingBoxGeometry();

		m_sceneNode_list.push_back(node_geometry);
	}

	scene::SceneNode* Importer::getSceneNode(unsigned int i)
	{
		if(i > m_sceneNode_list.size())
		{
			//utils::log() << "ERROR (Importer): wrong scene node index\n";
			return 0;
		}
		else
		{
			return m_sceneNode_list[i];
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
