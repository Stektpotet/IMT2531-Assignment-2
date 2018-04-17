#include <overkill/Entity.hpp>

namespace overkill
{
    Entity::Entity(C::Tag tag, glm::vec3 pos, glm::vec3 rot, glm::vec3 vel, glm::vec3 angVel)
    {
        m_tag = tag;
        m_position = pos;
        m_rotation = rot;
        m_velocity = vel;
        m_angularVelocity = angVel;
    }
    
    glm::vec3 Entity::getPosition()
    {   return m_position;  }

    glm::vec3 Entity::getRotation()
    {   return m_rotation;  }

    glm::vec3 Entity::getVelocity()
    {   return m_velocity;  }

    glm::vec3 Entity::getAngularVelocity()
    {   return m_angularVelocity;   }

    void Entity::setPosition(glm::vec3 pos)
    {   m_position = pos;   }

    void Entity::setRotation(glm::vec3 rot)
    {   m_rotation = rot;   }

    void Entity::setVelocity(glm::vec3 vel)
    {   m_velocity = vel;   }

    void Entity::setAngularVelocity(glm::vec3 angVel)
    {   m_angularVelocity = angVel; }

    void Entity::update()
    {
        m_position += m_velocity;
        m_rotation += m_angularVelocity;
    }

	auto Transform::modelToWorld() -> glm::mat4
	{
		auto m2w = glm::scale(glm::mat4(1), m_scale);

		m2w = glm::rotate(m2w, m_rotation.x, right);
		m2w = glm::rotate(m2w, m_rotation.y, up);
		m2w = glm::rotate(m2w, m_rotation.z, forward);

		m2w = glm::translate(m2w, m_position);
		return m2w;
	}
}