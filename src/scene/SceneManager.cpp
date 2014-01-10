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

  void SceneManager::init(void)
  {
      std::vector<GLuint*> m_texture_unit_list();
      m_texture_unit_counter = 0;
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

  void SceneManager::addTexture(GLuint texture_unit)
  {

  }

  GLuint SceneManager::getTexture(unsigned int index)
  {

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

  GLuint SceneManager::loadTexture(std::string filename)
  {
      std::cout << "    Loading " << filename << std::endl;
      GLuint new_texture_handle;
      m_texture_unit_list.push_back(new_texture_handle);

      m_texture_unit_list[m_texture_unit_counter] = SOIL_load_OGL_texture
      (
          filename.c_str(),
          SOIL_LOAD_AUTO,
          SOIL_CREATE_NEW_ID,
          SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
      );

      m_texture_unit_counter++;
      return m_texture_unit_list[m_texture_unit_counter-1];
  }
}
