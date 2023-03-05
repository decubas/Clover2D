#pragma once

#include <Clover.h>
#include <box2d/box2d.h>


class Entity;

struct Collision2DPoints {
	glm::vec2 Point;
	glm::vec2 Normal;
};

struct Collision2D {
	Collision2D(EntityID a, EntityID b, Collision2DPoints points)
		: entity(a), other(b), Points(points)
	{

	}
	EntityID entity;
	EntityID other;
	Collision2DPoints Points;
};

class Physics2DWorld;

//typedef void(*CollisionCallback)(Collision2D&);
typedef std::function<void(Collision2D&)> CollisionCallback;
class Collision2DObject : public Cl_Object
{
protected:
public:
	std::vector<CollisionCallback> m_onBeginCollision;
	std::vector<CollisionCallback> m_onExitCollision;


	b2BodyDef m_bodyDef;
	b2Body* m_body;

	void setTrigger(bool trigger);
	void setAngle(float angle);
	void setPosition(float x, float y);
	glm::vec2 Position();
	float Angle();


	template<class T>
	T* GetShape(int index = 0)
	{
		if(GetFixture(index))
			return (T*)GetFixture(index)->GetShape();

		return nullptr;
	}
	b2Fixture* GetFixture(int index = 0);

	void CallBeginCollision(Collision2D& collision);
	void CallExitCollision(Collision2D& collision);

};
