//! @file Material.h

#pragma once

#ifndef MATERIAL_H
#define MATERIAL_H

#include "../utils/Common.h"

namespace scene
{
    class Material
    {
        private:
            //! index
            unsigned int m_index_number;
            //! Diffuse
            glm::vec3 m_diffuse_color;
            int       m_diffuse_map_id;
            //! Specular
            glm::vec3 m_specular_color;
            int       m_specular_map_id;
            float     m_specular_highlight_exponent;
            int       m_cube_map_id;
            //! Normal/Bump
            int       m_normal_map_id;

        public:
            //! \brief Constructor: default
            Material();

            //! \brief Constructor
            Material(unsigned int index,
                     glm::vec3 diffuse_color, int diffuse_tex_id,
                     glm::vec3 sepcular_color, int specular_tex_id,
                     float specular_highlight_exponent,
                     int normal_map_id);

            //! \brief Returns the diffuse color
            glm::vec3* getDiffuseColor(void);

            //! \brief Returns the specular color
            glm::vec3* getSpecularColor(void);

            //! \brief Returns the materials index
            unsigned int getIndexNumber(void);
    };
}

#endif // MATERIAL_H
