//! @file SceneNode.cpp
#include "SceneNode.h"

namespace scene {

	SceneNode::SceneNode(nodetype type, unsigned int id, std::string name)
	{
		m_type	    = type;
		m_id	    = id;
		m_name	    = name;
		m_transform = Transform();
	}

	SceneNode::~SceneNode(void)
	{
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

	unsigned int SceneNode::getID(void)
	{
	  return m_id;
	}

	std::string SceneNode::getName(void)
	{
	  return m_name;
	}

	nodetype SceneNode::getType(void)
	{
	  return m_type;
	}


}
