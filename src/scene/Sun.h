//! @file Light.h
#pragma once
#ifndef SUN_H
#define SUN_H

#include "../utils/Common.h"
#include "../renderer/ShaderProgram.h"
#include "../renderer/FrameBufferObject.h"
#include "SceneNode.h"
#include "Light.h"
#include "Geometry.h"
#include <vector>
#define _USE_MATH_DEFINES
#include <math.h>

#include <glm/glm.hpp>
#include "../post/SlimQuad.h"

namespace scene
{
    class Sun : public Light
    {
        private:
			float cycleTime; //zwischen 0 und 1;
			glm::vec3 calcColor();
			float calcBloomAmount();
			float calcBlend();
			glm::vec3 calcAmounts();
			float blendSmooth1(float in);
        public:

			glm::vec3 color_night;
			glm::vec3 color_dawn;
			glm::vec3 color_day;
			glm::vec3 color_rise;
		
			glm::vec3 tar;
			glm::vec3 ssPos;
			int hour;
			int minute;
			int TexDay;
			int TexNight;
			float distance;
			float bloomAmount; // 0 = night // 1 = day
			float textureBlend; // 0 = night // 1 = day
			float speed;
			float ambientAmount;
			float diffuseAmount;
			float tone_factor;

			GLint sunTexture;
			SlimQuad* billboard;


			Sun(int id,
                 std::string name,
                 Transform transform,
                 glm::vec3 color,
                 float intensity,
				 float pDistance, 
				 int pHour,
				 GLint billboardTex);
        
			void render();    
			void update(glm::mat4 projection, glm::mat4 view);

			void setHour(int pHour);
			void incHour();
			void decHour();

			void incMinute();
			void decMinute();

			void setMinute(int pMinute);
			
			int getHour();

			int getMinute();
    };
}

#endif // SUN_H
