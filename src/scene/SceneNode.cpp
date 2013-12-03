//! @file SceneNode.cpp
#include "SceneNode.h"

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

	SceneNode::SceneNode(std::string name, unsigned int id)
	{
		m_name	= name;
		m_id	= id;
	}

	Transform* SceneNode::getTransform(void)
	{
		return &m_transform;
	}

	void SceneNode::setTransform(const glm::vec3& translatation, const glm::quat& rotation, const glm::vec3& scale)
	{
		m_transform.setPosition(translatation);
		m_transform.setRotation(rotation);
		m_transform.setScale(scale);
	}

	void SceneNode::setTransform(scene::Transform transform)
	{
		m_transform = transform;
	}
}
