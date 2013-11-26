//! @file Importer.h
#pragma once
#ifndef H_IMPORTER
#define H_IMPORTER

#include "../utils/Common.h"
#include "../scene/SceneNode.h"
#include "../scene/Geometry.h"
#include <glm/gtx/quaternion.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <vector>

//! @class Importer
namespace tone {
	namespace utils {
		//! @class Importer
		class Importer
		{
			public:
				static Importer* instance(void);
                void importFile(const std::string& pathToFile);
				void processScene(void);
				void processGeometry(const unsigned int& mesh_index, scene::Transform* node_transform);
				scene::SceneNode* getSceneNode(unsigned int i);
                scene::SceneNode* getSceneNode(const int index);
				void loadTexture(std::string filename);

			private:
				Importer(void);
				~Importer(void);
				const aiScene* m_aiScene;
				Assimp::Importer m_aiImporter;

                std::vector<scene::SceneNode*> m_sceneNode_list;
                std::vector<GLuint> m_texture_list;
		};
	}
}
#endif // H_IMPORTER
