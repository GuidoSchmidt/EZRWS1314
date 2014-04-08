//! @file SceneManager.h
#pragma once

#ifndef SCENEMANAGER_H
#define SCENEMANAGER_H

#include "SceneNode.h"
#include "Camera.h"
#include "Geometry.h"
#include "Light.h"



#include <map>
#include <SOIL/SOIL.h>
#include <FreeImage/FreeImage.h>

namespace scene
{
  class SceneManager
  {
    private:
      std::vector<SceneNode*>             m_scene_node_list;
      std::map<unsigned int, SceneNode*>  m_sceneNode_id_map;
      std::map<std::string, unsigned int> m_sceneNode_name_map;
      std::map<unsigned int, Material*>   m_material_index_map;
      std::vector<GLuint>                 m_texture_unit_list;
      unsigned int                        m_texture_unit_counter;
      std::map<unsigned int, Light*>      m_light_index_map;

      //! \brief Constructor: default
      SceneManager();
      void init();

    public:
      //! \brief Returns a singleton instance
      static SceneManager* instance(void);

      //! \brief Adds a scene node to the manager
      void addSceneNode(SceneNode* node);

      //! \brief Return a scene node by its index
      SceneNode* getNode(unsigned int index);

      //! \brief Return a scene node by its name
      SceneNode* getNode(std::string name);

      //! \brief Builds a simple render queue
      std::vector<Geometry*> generateRenderQueue(void);

      //! \brief Adds a material to the scene
      void addMaterial(Material* material);

      //! \brief Adds a texture
      void addTexture(GLuint texture_unit);

      //! \brief Adds a light source
      void addLight(Light* light);

      //! \brief Returns a light source by its id
      Light* getLight(unsigned int id);

      //! \brief Returns a texture handle by its index
      GLuint getTexture(unsigned int index);

	//! \brief Loads a texture from a given filepath and returns the texture handle
	GLuint loadTexture(std::string filename, bool repeat);

	//! \brief Loads a hdr texture from a given filepath and returns the texture handle
	GLuint loadHDRTexture(std::string filename);

	GLuint loadCubeMap(std::string filename,bool HDR);

	GLuint loadDDS(std::string filename);

	void deleteGeometryNode(const unsigned int index);
  };
}

#endif // SCENEMANAGER_H
