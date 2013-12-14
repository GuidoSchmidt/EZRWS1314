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
    };
}

#endif // MATERIAL_H
