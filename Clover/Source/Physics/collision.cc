
#include "collision.h"


	void Collision2DObject::setTrigger(bool trigger)
	{
		GetFixture()->SetSensor(trigger);
	}

	void Collision2DObject::setAngle(float angle)
	{
		m_bodyDef.angle = angle;

		if (m_body)
		{
			b2Vec2 pos = b2Vec2(Position().x, Position().y);
			m_body->SetTransform(pos, angle);
			m_body->SetAwake(true);
		}
	}

	void Collision2DObject::setPosition(float x, float y)
	{
		m_bodyDef.position = { x, y };

		if (m_body)
		{
			b2Vec2 pos = b2Vec2(x, y);
			m_body->SetTransform(pos, m_body->GetAngle());
			m_body->SetAwake(true);
		}
	}

	glm::vec2 Collision2DObject::Position()
	{
		return { m_body->GetPosition().x, m_body->GetPosition().y };
	}


	float Collision2DObject::Angle()
	{
		return m_body->GetAngle();
	}

	b2Fixture* Collision2DObject::GetFixture(int index /*= 0*/)
	{
		return m_body->GetFixtureList() + index;
	}


	void Collision2DObject::CallBeginCollision(Collision2D& collision)
	{
		for (int i = 0; i < m_onBeginCollision.size(); ++i)
		{
			m_onBeginCollision[i](collision);
		}
	}


	void Collision2DObject::CallExitCollision(Collision2D& collision)
	{
		for (int i = 0; i < m_onExitCollision.size(); ++i)
		{
			m_onExitCollision[i](collision);
		}
	}
