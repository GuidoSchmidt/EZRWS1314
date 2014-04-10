//! @file Material.cpp

#include "Material.h"

namespace scene
{
    Material::Material()
    {
    }

    Material::Material(unsigned int index,
                       glm::vec3 diffuse_color,
                       GLuint diffuse_tex_id,
                       glm::vec3 specular_color,
                       GLuint specular_tex_id,
                       float specular_exponent,
                       GLuint normal_map_id)
    {
        m_index_number      = index;
        m_diffuse_color     = diffuse_color;
        m_diffuse_map_ptr   = diffuse_tex_id;
        m_specular_color    = specular_color;
        m_specular_map_ptr  = specular_tex_id;
        m_specular_exponent = specular_exponent;
        m_normal_map_ptr    = normal_map_id;
    }

    unsigned int Material::getIndexNumber(void)
    {
        return m_index_number;
    }

    glm::vec3* Material::getDiffuseColor(void)
    {
        return &m_diffuse_color;
    }

    GLuint Material::getDiffuseTexture(void)
    {
        return m_diffuse_map_ptr;
    }

    glm::vec3* Material::getSpecularColor(void)
    {
        return &m_specular_color;
    }

	//! \brief
	void Material::setTranslucency(float translucency)
	{
		m_translucency = translucency;
	}

	//! \brief
	float Material::getTranslucency(void)
	{
		return m_translucency;
	}

    GLuint Material::getSpecularTexture(void)
    {
        return m_specular_map_ptr;
    }

    float Material::getShininess(void)
    {
        return m_specular_exponent;
    }

    GLuint Material::getNormalTexture(void)
    {
        return m_normal_map_ptr;
    }
}
