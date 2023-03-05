
#include "rigidbody.h"


	b2Vec2 Convert(glm::vec2 other)
	{
		return b2Vec2(other.x, other.y);
	}

	glm::vec2 Convert(b2Vec2  other)
	{
		return glm::vec2(other.x, other.y);
	}

	void RigidBody2D::AddForce(glm::vec2 force)
	{
		m_body->ApplyForceToCenter(Convert(force), true);
	}

	void RigidBody2D::AddForceAtLocation(glm::vec2 force, glm::vec2 position)
	{
		m_body->ApplyForce(Convert(force), m_bodyDef.position, true);
	}

	void RigidBody2D::setVelocity(float x, float y)
	{
		m_body->SetLinearVelocity({ x, y });
	}

	bool RigidBody2D::IsType(Type type)
	{
		return m_bodyDef.type == (b2BodyType)type;
	}


	void RigidBody2D::setType(Type type)
	{
		m_bodyDef.type = (b2BodyType)type;

		if (m_body)
		{
			m_body->SetType((b2BodyType)type);
		}
	}

	bool RigidBody2D::UseGravity()
	{
		return m_body->GetGravityScale() != 0.0f;
	}


	void RigidBody2D::setGravityScale(float scale)
	{
		m_bodyDef.gravityScale = scale;
	}


	void RigidBody2D::setMass(float mass)
	{
		b2MassData data;
		data.mass = mass;
		m_body->SetMassData(&data);
	}

	


	glm::vec2 RigidBody2D::Velocity()
	{
		return Convert(m_body->GetLinearVelocity());
	}

	float RigidBody2D::Mass()
	{
		return m_body->GetMass();
	}

	float RigidBody2D::GravityScale()
	{
		return m_body->GetGravityScale();
	}