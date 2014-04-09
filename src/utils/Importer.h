//! @file Importer.h
#pragma once
#ifndef H_IMPORTER
#define H_IMPORTER

#include "../utils/Common.h"

#include "../scene/SceneNode.h"
#include "../scene/Camera.h"
#include "../scene/Geometry.h"
#include "../scene/Material.h"
#include "../scene/Light.h"

#include <glm/gtx/quaternion.hpp>
#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <vector>
#include "../scene/SceneManager.h"

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

			std::vector<scene::Geometry*> m_geometry_node_list;
			std::vector<scene::Camera*>	  m_camera_node_list;
			std::vector<GLuint>           m_texture_list;

            GLuint tex_2d;

		public:
			//! \brief Returns the singleton instance
			static Importer* instance(void);

			//! \brief Imports a file from a given filepath and a folder name for the texture files
			/*
				CAUTION! texture_folder_name is the name of the folder inside "resources/textures/".
				Do not put the whole file-path string into the parameter, only the folder name!
			*/
			void importFile(const std::string& pathToFile, const std::string texture_folder_name);

			//! \brief Processes the loaded assimp scene
			void processScene(void);

			//! \brief Processes the loaded geometry from assimp scene
			void processGeometry(const unsigned int& mesh_index, scene::Transform* node_transform);

			//! \brief Helper function for loading textures from RESOURCES_PATH
			std::string getRelativeTexturePath(std::string filepath);

			//! \brief Helper function for loading compressed dds textures from RESOURCES_PATH
			std::string Importer::getRelativeTexturePathCompressed(std::string filepath);

			//! \brief Returns a geometry node of the scene by index
			scene::Geometry* getGeometryNode(const unsigned int index);

			void deleteGeometryNode(const unsigned int index);

			//! \brief Returns a camera of the scene by index
			scene::Camera* getCameraNode(const unsigned int index);
	};
}
#endif // H_IMPORTER
