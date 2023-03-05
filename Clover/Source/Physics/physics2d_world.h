#pragma once

#include <Clover.h>
#include <box2d/box2d.h>
#include "rigidbody.h"
#include "collider.h"


	struct RigidBody2DComponent;
	struct BoxCollider2DComponent;
	struct CircleCollider2DComponent;

	class Physics2DWorld {
	private:
		b2World* world;
		std::vector<b2Body*> bodies;
		b2Vec2 gravity = b2Vec2(0, -9.81f);

	public:
		Physics2DWorld();
		~Physics2DWorld();

		void Create();

		void CreateRigibBody2D(RigidBody2D* rb);
		void CreateBoxCollider2D(RigidBody2D& rb, Collider2D* boxCollider2D);
		void CreateCircleCollider2D(RigidBody2D& rb, Collider2D* circleCollider2D);

		void SetGravity(const glm::vec2& newGravity);
		const glm::vec2 Gravity();

		void Reset();
		
		void ImGuiEditor();

		b2World& GetWorld();

		void Step(float dt);
		void DebugDraw();

		static bool s_DrawDebug;
	};

	struct Physics2D
	{
		static Physics2DWorld* getWorld();
	};
