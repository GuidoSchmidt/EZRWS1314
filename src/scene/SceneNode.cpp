//! @file SceneNode.cpp
#include "SceneNode.h"

namespace ezr {
	namespace scene {
		SceneNode::SceneNode(unsigned int id)
		{
			m_name  = "";
			m_id	= id;
			m_transform = Transform();
		}

		SceneNode::SceneNode(void)
		{
			m_name  = "";
			m_id	= 0;
			m_transform = Transform();
		}

		SceneNode::~SceneNode(void)
		{
		}

		//! \brief Constructor
		SceneNode::SceneNode(std::string name, unsigned int id)
		{
			m_name	= name;
			m_id	= id;
		}

		//! \brief Returns the model matrix for the node
		Transform* SceneNode::getTransform(void)
		{
			return &m_transform;
		}

		//! \brief Sets the nodes transform
		void SceneNode::setTransform(const glm::vec3& translatation, const glm::quat& rotation, const glm::vec3& scale)
		{
			m_transform.setPosition(translatation);
			m_transform.setRotation(rotation);
			m_transform.setScale(scale);
		}

		//! \brief Sets the nodes transform
		void SceneNode::setTransform(scene::Transform transform)
		{
			m_transform = transform;
		}
	}
}
