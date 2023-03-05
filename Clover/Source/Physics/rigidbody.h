#pragma once

#include <Clover.h>
#include "collision.h"


	class RigidBody2D : public Collision2DObject
	{
	public:
		enum class Type : int32 
		{
			Dynamic = b2BodyType::b2_dynamicBody, 
			Static = b2BodyType::b2_staticBody, 
			Kinematic = b2BodyType::b2_kinematicBody
		} m_Type = Type::Dynamic;

		RigidBody2D() = default;
		RigidBody2D::RigidBody2D(Type type)
			: m_Type(type)
		{}
		bool bFixedRotation = false;

		void AddForce(glm::vec2 force);
		void AddForceAtLocation(glm::vec2 force, glm::vec2 position);

		void setVelocity(float x, float y);
		void setType(Type type);
		void setGravityScale(float scale);
		void setMass(float mass);
		
		
		bool UseGravity();
		bool IsType(Type type);
		glm::vec2 Velocity();
		float Mass();
		float GravityScale();

	private:
	};
