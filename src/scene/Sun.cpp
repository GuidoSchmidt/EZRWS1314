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
		int pHour,
		GLint billboardTex)
		: Light(id, name, transform, color, intensity)
	{
		color_night = glm::vec3(0, 0.1, 0.1);
		color_dawn = glm::vec3(0.56, 0.26, 0.4);
		color_day = glm::vec3(1, 1, 0.9);
		color_rise = glm::vec3(0.6, 0.12, 0.2);

		ambientAmount = 0.5;
		diffuseAmount = 0.5;

		hour = pHour;
		minute = 0;
		cycleTime = (float)hour / 24.0f;
		textureBlend = 0.5;
		billboard = new SlimQuad();
		sunTexture = billboardTex;
		distance = pDistance;

		tone_factor = 0;


		m_transform.setScale(glm::vec3(300));
	}

	void Sun::incHour()
	{
		if (hour == 23)
			hour = 0;
		else
			hour++;
		cycleTime = (float)hour / 24.0f + ((float)minute / 60.0f) / 24.0f;

	}

	void Sun::decHour()
	{
		if (hour == 0)
			hour = 23;
		else
			hour--;
		cycleTime = (float)hour / 24.0f + ((float)minute / 60.0f) / 24.0f;

	}

	void Sun::incMinute()
	{
		if (minute == 59) {
			minute = 0;
			incHour();
		}
		else
			minute++;
		cycleTime = (float)hour / 24.0f + ((float)minute / 60.0f) / 24.0f;
	}

	void Sun::decMinute()
	{
		if (minute == 0) {
			minute = 59;
			decHour();
		}
		else
			minute--;
		cycleTime = (float)hour / 24.0f + ((float)minute / 60.0f) / 24.0f;

	}

	void Sun::setHour(int pHour)
	{
		hour = pHour;
		cycleTime = (float)hour / 24.0f + ((float)minute / 60.0f) / 24.0f;
	}

	void Sun::setMinute(int pMinute)
	{
		minute = pMinute;
		cycleTime = (float)hour / 24.0f + ((float)minute / 60.0f) / 24.0f;
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
		float cycle = cycleTime * 2 * M_PI;
		//6uhr aufgang (1,0,0)
		//12uhr zenit (0,1,0)
		//18uhr untergang (-1,0,0)

		glm::vec3 pos;
		pos.x = distance*sin(cycle);
		pos.y = distance*-cos(cycle);
		pos.z = 0.0;
		m_transform.setPosition(pos);
		/*glm::mat4 rot1 = glm::mat4(0, 0.0f, 1, 0.0f,
		0.0f, 1.0f, 0.0f, 0.0f,
		-1, 0.0f, 0, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f);
		glm::mat4 rot2 = glm::mat4(cos(-cycle + M_PI_2), -sin(-cycle + M_PI_2), 0, 0.0f,
		sin(-cycle + M_PI_2), cos(-cycle + M_PI_2), 0.0f, 0.0f,
		-0, 0.0f, 1, 0.0f,
		0.0f, 0.0f, 0.0f, 1.0f);*/

		glm::mat4 rot = glm::mat4(0, 0.0f, 1, 0.0f,     // = rot1* rot2
			sin(-cycle + M_PI_2), cos(-cycle + M_PI_2), 0, 0,
			cos(-cycle + M_PI_2), -sin(-cycle + M_PI_2), 0, 0,
			0.0f, 0.0f, 0.0f, 1.0f);

		m_transform.setRotation(rot);
		//m_transform.setRotation(rot2*rot1);

		//m_transform.setRotation(glm::lookAt(glm::vec3(0.0, 0.0, 0.0), pos, glm::vec3(0.0, 1.0, 0.0001)));


		//m_transform.setRotation(glm::rotate(m_transform.getModelMatrix(), (float)M_PI / 2, glm::vec3(0, 1, 0)));
		//calc ssPos
		ssPos = glm::vec3(projection * view * glm::vec4(pos, 1));
		ssPos.x = (ssPos.x / ssPos.z) / 2.0f + 0.5f;
		ssPos.y = (ssPos.y / ssPos.z) / 2.0f + 0.5f;



		//calc color
		m_color = calcColor() *m_intensity;

		//calc bloom
		bloomAmount = calcBloomAmount();

		//calc blend
		textureBlend = calcBlend();

		//lighting parameters
		glm::vec3 p = calcAmounts();
		ambientAmount = p.x;
		diffuseAmount = p.y;

		tone_factor = p.z;
		//std::cout << "time: " << hour << ":" << minute << "\t factor : " << tone_factor << std::endl;
	}

	void Sun::render()
	{
		//vorher den shader binden und die modelmatrix benutzen
		//m_transform.setRotation(glm::rotate(float( M_PI/2), glm::vec3(0, 1, 0)));
		billboard->draw();
	}

	glm::vec3 Sun::calcAmounts() {
		//vec2 (ambient,diffuse,tonefactor)
		if (hour <= 5 || hour >= 18)
		{
			return glm::vec3(1, 0, 3);
		}
		if (hour >= 6 || hour <= 17)
		{
			float blend = -cos((cycleTime * 2 - 0.5) * 2 * M_PI) / 2 + 0.5;
			return glm::vec3(1, blend, 3 - 3 * blend);
		}
	}

	//tag 1 0 3

	glm::vec3 Sun::calcColor()
	{
		float blend;

		if (hour <= 4 || hour >= 19)
		{
			return color_night; //nacht

		}
		else if (hour == 5)
		{
			blend = blendSmooth1((float)minute / 60.0f);
			return (1 - blend)*color_night + blend*color_rise;//blend 
		}
		else if (hour == 6)
		{
			blend = blendSmooth1((float)minute / 60.0f);
			return (1 - blend)*color_rise + blend*color_day;//blend
		}
		else if (hour >= 7 && hour <= 16)
		{
			return color_day; //morgen			
		}
		else if (hour == 17)
		{
			blend = blendSmooth1((float)minute / 60.0f);
			return (1 - blend)*color_day + blend*color_dawn;//blend
		}
		else if (hour == 18)
		{
			blend = blendSmooth1((float)minute / 60.0f);
			return (1 - blend)*color_dawn + blend*color_night;//blend		
		}
	}

	float Sun::blendSmooth1(float in)
	{
		// in must be between 0 and 1

		//out is between 0 and 1 but smooth
		return (float)(-cos(in * M_PI) / 2.0 + 0.5);
	}

	float Sun::calcBloomAmount()
	{
		if (hour < 6 || hour > 17)
			return 0.0;
		else if (hour == 6)
		{
			return blendSmooth1((float)minute / 60.0f);;
		}
		else if (hour > 6 && hour < 17)
			return 1;

		else if (hour == 17)
		{
			return 1 - blendSmooth1((float)minute / 60.0f);;
		}

	}

	float Sun::calcBlend()
	{
		//könnte man noch sweet mit eas-in-eas-out machen
		if (cycleTime >= 0.17 && cycleTime < 0.27)
		{
			//blend aufgang
			//1/8 =>0
			//3/8 =>1
			return blendSmooth1(10 * (cycleTime - 0.17f));

		}
		else if (cycleTime >= 0.27 && cycleTime < 0.73)
		{
			//tag
			return 1;
		}
		else if (cycleTime >= 0.73 && cycleTime < 0.83)
		{
			//blend untergang
			//5/8 =>1
			//7/8 =>0
			return 1 - blendSmooth1(10 * (cycleTime - 0.73f));
		}
		else
		{
			// nacht
			return 0;
		}

	}

}
