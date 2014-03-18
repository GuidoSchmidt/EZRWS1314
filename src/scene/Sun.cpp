//! @file Light.cpp
#include "Light.h"
#include "Sun.h"


namespace scene
{
    Sun::Sun(int id,
                 std::string name,
                 Transform transform,
                 glm::vec3 color,
                 float intensity,
				 float pDistance,
				 GLint billboardTex)
        : Light(id, name, transform, color, intensity)
    {
		color_night   = glm::vec3(0,0.13,0.26);
		color_dawn    = glm::vec3(0.56,0.26,0.4);
		color_day     = glm::vec3(1,1,0.9);
		color_rise    = glm::vec3(0.6,0.12,0.2);

		min = 1/1440;
		hour = 12;
		minute = 0;
		textureBlend = 0.5;
		billboard = new SlimQuad();
		sunTexture = billboardTex; 
		distance = pDistance;
    }


	void Sun::setHour(int pHour)
	{
		hour = pHour%24;
		cycleTime = (float)hour/24.0f + ((float)minute/60.0f)/24.0f;
	}

	void Sun::setMinute(int pMinute)
	{
		minute=pMinute%60;
		cycleTime = (float)hour/24.0f + ((float)minute/60.0f)/24.0f;
	}

	int Sun::getHour()
	{
		return hour;
	}

	int Sun::getMinute()
	{
		return minute;
	}

	void Sun::update(glm::mat4 projection, glm::mat4 view)
	{
		float cycle = cycleTime * 2*M_PI;
		//6uhr aufgang (1,0,0)
		//12uhr zenit (0,1,0)
		//18uhr untergang (-1,0,0)
			
		glm::vec3 pos;
		pos.x = distance*sin(cycle);
		pos.y = distance*-cos(cycle);
		pos.z = 0.0;
		m_transform.setPosition(pos);
			
		//calc ssPos
		ssPos = glm::vec3(projection * view * glm::vec4(pos,1));
		ssPos.x=(ssPos.x/ssPos.z)/2.0f+0.5f;
		ssPos.y=(ssPos.y/ssPos.z)/2.0f+0.5f;
			
			
		//m_transform.setRotation(glm::lookAt(pos,glm::vec3(0.0,0.0,0.0),glm::vec3(0.0,1.0,0.0)));
			
		//calc color
		m_color = calcColor() *m_intensity;
			
		//calc blend
		textureBlend = calcBlend();


	}
		
	void Sun::render()
	{	
		//vorher den shader binden und die modelmatrix benutzen
		billboard->draw();
	}	

	glm::vec3 Sun::calcColor()
	{
		float blend;
			
		if (hour < 5 || hour >= 19)
		{
			return color_night; //nacht
				
		}
		else if (hour >= 5 && hour < 6)
		{
			blend = (float)minute/60.0f;
			return (1-blend)*color_night+blend*color_rise;//blend 
		}
		else if (hour >= 6 && hour < 7)
		{
			return color_rise; //morgen			
		}
		else if (hour >= 7 && hour < 8)
		{
			blend = (float)minute/60.0f;
			return (1-blend)*color_rise+blend*color_day;//blend
		}
		else if (hour >= 8 && hour < 17)
		{
			return color_day; //morgen			
		}
		else if (hour >= 17 && hour < 18)
		{
			blend = (float)minute/60.0f;
			return (1-blend)*color_day+blend*color_dawn;//blend
		}
		else if (hour >= 18 && hour < 19)
		{
			blend = (float)minute/60.0f;
			return (1-blend)*color_dawn+blend*color_night;//blend		
		}
	}

	float Sun::calcBlend()
	{
		//könnte man noch sweet mit eas-in-eas-out machen
		if (cycleTime >= 0.125 && cycleTime < 0.375)
		{
			//blend aufgang
			//1/8 =>0
			//3/8 =>1
			return 4*cycleTime-0.5f;
				
		}
		else if (cycleTime >= 3.0/8.0 && cycleTime < 5.0/8.0)
		{
			//tag
			return 1;
		}
		else if (cycleTime >= 5.0/8.0 && cycleTime < 7.0/8.0)
		{
			//blend untergang
			//5/8 =>1
			//7/8 =>0
			return -4*cycleTime+3.5f;
		}
		else// (cycleTime <= 1/8 && cycleTime >= 7/8)
		{
			// nacht
			return 0;
		}
		
	}
    
}
