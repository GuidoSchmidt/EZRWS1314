//! @file Importer.h
#pragma once
#ifndef H_IMPORTER
#define H_IMPORTER

#include "../utils/Common.h"

#include "../scene/SceneNode.h"
#include "../scene/Camera.h"
#include "../scene/Geometry.h"

#include <glm/gtx/quaternion.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <vector>

namespace utils {
	//! @class Importer
	class Importer
	{
		private:
			//! \brief Constructor (singleton)
			Importer(void);

			//! \brief Destructor (singleton)
			~Importer(void);

			const aiScene*	 m_aiScene;
			Assimp::Importer m_aiImporter;

                        std::vector<scene::SceneNode*> m_sceneNode_list;
                        std::vector<GLuint>            m_texture_list;

		public:
			//! \brief Returns the singleton instance
			static Importer* instance(void);

			//! \brief Imports a file from a given filepath
			void importFile(const std::string& pathToFile);

			//! \brief Processes the loaded assimp scene
			void processScene(void);

			//! \brief Processes the readed geometry from assimp scene
			void processGeometry(const unsigned int& mesh_index, scene::Transform* node_transform);

			//! \brief Returns a scene node by index
			scene::SceneNode* getSceneNode(const int index);

			//! \brief Loads a texture from a given filepath
			void loadTexture(std::string filename);
	};
}
#endif // H_IMPORTER
