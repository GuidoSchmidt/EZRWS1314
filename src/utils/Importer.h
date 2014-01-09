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
#include <FreeImage/FreeImage.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <vector>
#include <SOIL/SOIL.h>

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


		public:
			//! \brief Returns the singleton instance
			static Importer* instance(void);

			//! \brief Imports a file from a given filepath
			void importFile(const std::string& pathToFile);

			//! \brief Processes the loaded assimp scene
			void processScene(void);

			//! \brief Processes the loaded geometry from assimp scene
			void processGeometry(const unsigned int& mesh_index, scene::Transform* node_transform);

			//! \brief Returns a geometry node of the scene by index
			scene::Geometry* getGeometryNode(const unsigned int index);

			//! \brief Returns a camera of the scene by index
			scene::Camera* getCameraNode(const unsigned int index);

			//! \brief Loads a texture from a given filepath and returns the texture handle
			GLuint loadTexture(std::string filename, bool repeat);

			//! \brief Loads a hdr texture from a given filepath and returns the texture handle
			GLuint loadHDRTexture(std::string filename);

			GLuint loadCubeMap(std::string filename,bool HDR);
	};
}
#endif // H_IMPORTER
