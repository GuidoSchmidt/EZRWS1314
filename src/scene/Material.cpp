//! @file Material.cpp

#include "Material.h"

namespace scene
{
    Material::Material()
    {
    }

    Material::Material(glm::vec3 diffuse_color, int diffuse_tex_id,
                       glm::vec3 specular_color, int specular_tex_id,
                       float specular_highlight_exponent,
                       int normal_map_id)
    {
        m_diffuse_color     = diffuse_color;
        m_diffuse_map_id    = diffuse_tex_id;
        m_specular_color    = specular_color;
        m_specular_highlight_exponent = specular_highlight_exponent;
        m_normal_map_id     = normal_map_id;
    }
}
