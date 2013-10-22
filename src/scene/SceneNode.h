//! @file SceneNode.h
#pragma once
#ifndef H_SCENENODE
#define H_SCENENODE

#include "../utils/Common.h"
#include "Transform.h"

namespace tone {
	namespace scene {
		enum nodetype {
			GEOMETRY,
			CAMERA,
			LIGHT
		};

		//! @class SceneNode
		class SceneNode
		{
			public:
				SceneNode(unsigned int id);
				SceneNode(std::string m_name, unsigned int m_id);
				~SceneNode(void);
				void setTransform(const glm::vec3& translatation, const glm::quat& rotation, const glm::vec3& scale);
				void setTransform(scene::Transform transform);
				Transform* getTransform(void);
				virtual nodetype* getType(void) = 0;
		
			protected:
				SceneNode(void);
				nodetype m_type;
				std::string m_name;
				unsigned  int m_id;
				Transform m_transform;
		};
	}
}
#endif //H_SCENENODE
