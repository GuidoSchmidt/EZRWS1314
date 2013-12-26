//! @file SceneManager.h
#pragma once

#ifndef SCENEMANAGER_H
#define SCENEMANAGER_H

#include "SceneNode.h"
#include "Geometry.h"
#include "Camera.h"
#include <map>

namespace scene
{
  class SceneManager
  {
    private:
      std::vector<SceneNode*>             m_scene_node_list;
      std::map<unsigned int, SceneNode*>  m_sceneNode_id_map;
      std::map<std::string, unsigned int> m_sceneNode_name_map;
      std::map<unsigned int, Material*>   m_material_index_map;

      //! \brief Constructor: default
      SceneManager();

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
  };
}

#endif // SCENEMANAGER_H
