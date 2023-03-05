#pragma once

#include <Clover.h>
#include <box2d/box2d.h>
#include <Scene/scene_manager.h>
#include "collision.h"

	b2Vec2 ConvertTo(glm::vec2 other)
	{
		return b2Vec2(other.x, other.y);
	}

	glm::vec2  ConvertTo(b2Vec2  other)
	{
		return glm::vec2(other.x, other.y);
	}

	class ContactListener : public b2ContactListener
	{
	public:
		virtual void BeginContact(b2Contact* contact) override
		{
			EntityID a = *(EntityID*)contact->GetFixtureA()->GetBody()->GetUserData().pointer;
			EntityID b = *(EntityID*)contact->GetFixtureB()->GetBody()->GetUserData().pointer;

			Collision2DPoints points;
			points.Normal = ConvertTo(contact->GetManifold()->localNormal);
			points.Point = ConvertTo(contact->GetManifold()->localPoint);
			Collision2D col (a, b, points);
			/*
			Entity e1 = { SCENE.GetCurrent(), a };
			Entity e2 = { SCENE.GetCurrent(), b };
			{
				if (e1.hasComponent<RigidBody2DComponent>())
				{
					e1.getComponent<RigidBody2DComponent>().rigidbody.CallBeginCollision(col);
				}

				if (e2.hasComponent<RigidBody2DComponent>())
				{
					e2.getComponent<RigidBody2DComponent>().rigidbody.CallBeginCollision(col);
				}
			}
			{
				if (e1.hasComponent<BoxCollider2DComponent>())
				{
					e1.getComponent<BoxCollider2DComponent>().collider.CallBeginCollision(col);
				}

				if (e2.hasComponent<BoxCollider2DComponent>())
				{
					e2.getComponent<BoxCollider2DComponent>().collider.CallBeginCollision(col);
				}
			}
			{
				if (e1.hasComponent<CircleCollider2DComponent>())
				{
					e1.getComponent<CircleCollider2DComponent>().collider.CallBeginCollision(col);
				}

				if (e2.hasComponent<CircleCollider2DComponent>())
				{
					e2.getComponent<CircleCollider2DComponent>().collider.CallBeginCollision(col);
				}
			}*/
		}

		virtual void EndContact(b2Contact* contact) override
		{
			EntityID a = *(EntityID*)contact->GetFixtureA()->GetBody()->GetUserData().pointer;
			EntityID b = *(EntityID*)contact->GetFixtureB()->GetBody()->GetUserData().pointer;

			Collision2DPoints points;
			points.Normal = ConvertTo(contact->GetManifold()->localNormal);
			points.Point = ConvertTo(contact->GetManifold()->localPoint);
			Collision2D col(a, b, points);
			/*
			Entity e1 = { SCENE.GetCurrent(), a };
			Entity e2 = { SCENE.GetCurrent(), b };
			{
				if (e1.hasComponent<RigidBody2DComponent>())
				{
					e1.getComponent<RigidBody2DComponent>().rigidbody.CallExitCollision(col);
				}

				if (e2.hasComponent<RigidBody2DComponent>())
				{
					e2.getComponent<RigidBody2DComponent>().rigidbody.CallExitCollision(col);
				}
			}
			{
				if (e1.hasComponent<BoxCollider2DComponent>())
				{
					e1.getComponent<BoxCollider2DComponent>().collider.CallExitCollision(col);
				}

				if (e2.hasComponent<BoxCollider2DComponent>())
				{
					e2.getComponent<BoxCollider2DComponent>().collider.CallExitCollision(col);
				}
			}
			{
				if (e1.hasComponent<CircleCollider2DComponent>())
				{
					e1.getComponent<CircleCollider2DComponent>().collider.CallExitCollision(col);
				}

				if (e2.hasComponent<CircleCollider2DComponent>())
				{
					e2.getComponent<CircleCollider2DComponent>().collider.CallExitCollision(col);
				}
			}
			*/

		}

		/// This is called after a contact is updated. This allows you to inspect a
		/// contact before it goes to the solver. If you are careful, you can modify the
		/// contact manifold (e.g. disable contact).
		/// A copy of the old manifold is provided so that you can detect changes.
		/// Note: this is called only for awake bodies.
		/// Note: this is called even when the number of contact points is zero.
		/// Note: this is not called for sensors.
		/// Note: if you set the number of contact points to zero, you will not
		/// get an EndContact callback. However, you may get a BeginContact callback
		/// the next step.
		virtual void PreSolve(b2Contact* contact, const b2Manifold* oldManifold) override
		{
			B2_NOT_USED(contact);

			B2_NOT_USED(oldManifold);
		}

		/// This lets you inspect a contact after the solver is finished. This is useful
		/// for inspecting impulses.
		/// Note: the contact manifold does not include time of impact impulses, which can be
		/// arbitrarily large if the sub-step is small. Hence the impulse is provided explicitly
		/// in a separate data structure.
		/// Note: this is only called for contacts that are touching, solid, and awake.
		virtual void PostSolve(b2Contact* contact, const b2ContactImpulse* impulse) override
		{
			B2_NOT_USED(contact);
			B2_NOT_USED(impulse);
		}
	};

	static ContactListener s_Box2DContactListener;
