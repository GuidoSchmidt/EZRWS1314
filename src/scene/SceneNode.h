//! @file SceneNode.h
#pragma once
#ifndef H_SCENENODE
#define H_SCENENODE

#include "../utils/Common.h"
#include "Transform.h"

namespace scene {
	enum nodetype {
		EMPTY = -1,
		GEOMETRY = 0,
		CAMERA = 1,
		LIGHT = 2,
	};

	//! @class SceneNode
	class SceneNode
	{
		protected:
			nodetype     m_type;
			std::string  m_name;
			unsigned int m_id;
			Transform    m_transform;

		public:
			//! \brief Constructor
			SceneNode(nodetype type, unsigned int id = -1, std::string name = "");
			
			//! \brief Constructor
			~SceneNode(void);

			//! \brief Sets the transform of the node
			void setTransform(const glm::vec3& translatation, const glm::quat& rotation, const glm::vec3& scale);
			
			//! \brief Sets the transform of the node
			void setTransform(scene::Transform transform);

			//! \brief Returns the nodes transform
			Transform* getTransform(void);

			//! \brief Returns the node type
			nodetype getType(void);

			//! \brief Returns the ID of the node
			unsigned int getID(void);

			//! \brief Returns the name of the node
			std::string getName(void);
	};
}
#endif //H_SCENENODE
