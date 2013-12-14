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
      std::vector<SceneNode*> m_scene_node_list;
      std::map<unsigned int, SceneNode*>  m_sceneNode_id_map;
      std::map<std::string, unsigned int> m_sceneNode_name_map;

      //! \brief Constructor: default
      SceneManager();

    public:
      //! \brief Returns a singleton instance
      static SceneManager* instance(void);

      //! \brief Adds a scene node to the manager
      void addSceneNode(SceneNode* node);

      //! \brief Return a scene node
      SceneNode* getNode(unsigned int i);
  };
}

#endif // SCENEMANAGER_H
