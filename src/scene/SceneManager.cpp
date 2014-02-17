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

  void SceneManager::addLight(Light* light)
  {
      m_light_index_map[light->getID()] = light;
  }

  Light* SceneManager::getLight(unsigned int id)
  {
      return m_light_index_map.at(id);
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


  	void SceneManager::deleteGeometryNode(const unsigned int index)
	{
		if (index > m_scene_node_list.size())
		{
			std::cerr << "ERROR (Importer): wrong scene node index" << std::endl;
			return;
		}
		else
		{
			m_scene_node_list.erase(m_scene_node_list.begin()+index);
		}
	}


	GLuint SceneManager::loadTexture(std::string filename, bool repeat)
	{
		GLuint tex_2d;
			
		glActiveTexture(GL_TEXTURE0);	
		glGenTextures( 1, &tex_2d );
		glBindTexture( GL_TEXTURE_2D, tex_2d );

		tex_2d = SOIL_load_OGL_texture
		(
			filename.c_str(),
			SOIL_LOAD_AUTO,
			SOIL_CREATE_NEW_ID,
			SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
		);
		if (repeat)
		{
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		}
		else
		{
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
		}

		return tex_2d;
	}

	GLuint SceneManager::loadCubeMap(std::string filename, bool HDR) 
	{
		GLuint cube_map;
		glGenTextures(1, &cube_map);
		glBindTexture(GL_TEXTURE_CUBE_MAP, cube_map);
		glActiveTexture(GL_TEXTURE0);
		
		std::string sides[6] = { "_pos_x", "_neg_x", "_pos_y", "_neg_y", "_pos_z", "_neg_z"};
		for (int i = 0; i < 6; i++)
		{
			// load a HDR RGB Float imager
			std::string sname = filename + sides[i] + ((HDR) ? ".exr" : ".jpg");
			const char *name = sname.c_str();
			FREE_IMAGE_FORMAT format = FreeImage_GetFileType(name, 0); //should be FIF_HDR or FIF_EXR
			FIBITMAP *src = FreeImage_Load(format, name, 0);
			int depth = FreeImage_GetBPP(src);
			FREE_IMAGE_TYPE type = FreeImage_GetImageType(src); //should be FIT_RGBF or FIT_BITMAP
			FREE_IMAGE_COLOR_TYPE color = FreeImage_GetColorType(src); //should be FIC_RGB
			BYTE *bits = (BYTE*)FreeImage_GetBits(src);

			if (HDR)
			{
				FIRGBF *rgbFsrc = (FIRGBF*)bits;
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB32F, FreeImage_GetWidth(src), FreeImage_GetHeight(src), 0, GL_RGB, GL_FLOAT, rgbFsrc);
			}
			else {
				glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB8, FreeImage_GetWidth(src), FreeImage_GetHeight(src), 0, GL_RGB, GL_UNSIGNED_BYTE, bits);
			}
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP);
			glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP);

			FreeImage_Unload(src);
		}
		glBindTexture(GL_TEXTURE_CUBE_MAP, 0);

		return cube_map;
	}

  GLuint SceneManager::loadHDRTexture(std::string filename) {
		GLuint tex_2d;

		glActiveTexture(GL_TEXTURE0);
		glGenTextures(1, &tex_2d);
		glBindTexture(GL_TEXTURE_2D, tex_2d);
		

		// load a HDR RGB Float image
		FREE_IMAGE_FORMAT format = FreeImage_GetFileType(filename.c_str(),0); //should be FIF_HDR or FIF_EXR
		FIBITMAP *src = FreeImage_Load(format, filename.c_str(), 0);
		int depth = FreeImage_GetBPP(src);
		FREE_IMAGE_TYPE type = FreeImage_GetImageType(src); //should be FIt_RGBF
		FREE_IMAGE_COLOR_TYPE color = FreeImage_GetColorType(src); //should be FIC_RGB
		BYTE *bits = (BYTE*)FreeImage_GetBits(src);
		FIRGBF *rgbFsrc = (FIRGBF*)bits;
		
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB32F, FreeImage_GetWidth(src), FreeImage_GetHeight(src), 0, GL_RGB, GL_FLOAT, rgbFsrc);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

		FreeImage_Unload(src);
		return tex_2d;

	}
}
