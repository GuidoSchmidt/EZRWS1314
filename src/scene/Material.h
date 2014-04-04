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
            GLuint    m_diffuse_map_ptr;
            //! Specular
            glm::vec3 m_specular_color;
            GLuint    m_specular_map_ptr;
            float     m_specular_exponent;
            int       m_cube_map_id;
            //! Normal/Bump
            GLuint    m_normal_map_ptr;
            //! Translucency
            GLuint    m_translucency_map_ptr;

        public:
            //! \brief Constructor: default
            Material();

            //! \brief Constructor
            Material(unsigned int index,
                     glm::vec3 diffuse_color,
                     GLuint diffuse_tex_id,
                     glm::vec3 sepcular_color,
                     GLuint specular_tex_id,
                     float specular_highlight_exponent,
                     GLuint normal_map_id,
                     GLuint translucency_map_id);

            //! \brief Returns the materials index
            unsigned int getIndexNumber(void);

            //! \brief Returns the diffuse color
            glm::vec3* getDiffuseColor(void);

            //! \brief Returns the diffuse texture handle
            GLuint getDiffuseTexture(void);

            //! \brief Returns the specular color
            glm::vec3* getSpecularColor(void);

            //! \brief Returns the specular texture handle
            GLuint getSpecularTexture(void);

            //! \brief Returns the specular exponent
            float getShininess(void);

            //! \brief Returns the specular texture handle
            GLuint getNormalTexture(void);

            //! \brief Returns the translucency texture handle
            GLuint getTranslucencyTexture(void);
    };
}

#endif // MATERIAL_H
