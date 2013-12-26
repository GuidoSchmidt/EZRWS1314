//! @file SceneManager.cpp
#include "SceneManager.h"
#include "Camera.h"

namespace scene
{
  SceneManager::SceneManager()
  {
  }

  SceneManager* SceneManager::instance(void)
  {
          static SceneManager m_scene_manager;
          return &m_scene_manager;
  }

  void SceneManager::addSceneNode(SceneNode* node)
  {
    m_scene_node_list.push_back(node);
    m_sceneNode_id_map[node->getID()] = node;
    m_sceneNode_name_map[node->getName()] = node->getID();
  }

  SceneNode* SceneManager::getNode(unsigned int index)
  {
    return m_sceneNode_id_map[index];
  }

  SceneNode* SceneManager::getNode(std::string name)
  {
    return m_sceneNode_id_map[m_sceneNode_name_map.at(name)];
  }

  void SceneManager::addMaterial(Material* material)
  {
      m_material_index_map[material->getIndexNumber()] = material;
  }

  std::vector<Geometry*> SceneManager::generateRenderQueue(void)
  {
    std::vector<Geometry*> render_queue;
    for(unsigned int node = 0; node < m_scene_node_list.size(); node++)
    {
        if(m_scene_node_list.at(node)->getType() == GEOMETRY)
        {
            Geometry* current = static_cast<Geometry*>(m_scene_node_list.at(node));
            current->setMaterialTo(m_material_index_map.at(current->getMaterialIndex()));
            render_queue.push_back( current );
        }
        else
        {
            continue;
        }
    }

    return render_queue;
  }
}
