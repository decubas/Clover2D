
#include <Physics/physics2d_world.h>
#include <Physics/contact_listener.h>
#include <scene/scene_manager.h>
#include "CloverEngine.h"
#include "Graphics/UI.h"

	static Physics2DWorld* s_World = nullptr;
	static std::vector<UniqueID> s_id;
	static bool initialized = false;
	bool Physics2DWorld::s_DrawDebug = false;
	//class GLFWwindow* g_mainWindow;

	Physics2DWorld::Physics2DWorld()
	{
		Create();
	}


	Physics2DWorld::~Physics2DWorld()
	{
		delete s_World;
		s_World = nullptr;
	}


	void Physics2DWorld::Create()
	{
		world = new b2World(gravity);
		s_World = this;

		//g_mainWindow = (GLFWwindow*)CloverEngine::GetEngine()->m_Window->GetNativeWindow();
		//g_debugDraw.Create();
		uint32 flags = 0;
		flags += b2Draw::e_shapeBit;
		flags += b2Draw::e_jointBit;
		flags += b2Draw::e_aabbBit;
		flags += b2Draw::e_centerOfMassBit;
		//g_debugDraw.SetFlags(flags);
		world->SetContactListener(&s_Box2DContactListener);
		world->SetGravity(gravity);
		//world->SetDebugDraw(&g_debugDraw);
	}

	void Physics2DWorld::CreateRigibBody2D(RigidBody2D* rb)
	{
		rb->m_body = world->CreateBody(&rb->m_bodyDef);
		s_id.push_back(rb->GetInternalID());
		rb->m_body->GetUserData().pointer = reinterpret_cast<uintptr_t>(rb);
		bodies.push_back(rb->m_body);
	}

	void Physics2DWorld::CreateBoxCollider2D(RigidBody2D& rb, Collider2D* boxCollider2D)
	{
		b2PolygonShape polygonShape;
		polygonShape.SetAsBox(boxCollider2D->Size.x, boxCollider2D->Size.y);
		const glm::vec2 position = boxCollider2D->Offset + rb.Position();
		rb.setPosition(position.x, position.y);
		boxCollider2D->m_body = rb.m_body;
		b2FixtureDef fixtureDef;
		fixtureDef.shape = &polygonShape;
		fixtureDef.density = boxCollider2D->Density;
		fixtureDef.friction = boxCollider2D->Friction;
		rb.m_body->CreateFixture(&fixtureDef);
	}

	void Physics2DWorld::CreateCircleCollider2D(RigidBody2D& rb, Collider2D* circleCollider2D)
	{
		b2CircleShape circleShape;
		circleShape.m_radius = circleCollider2D->Radius;
		const glm::vec2 position = circleCollider2D->Offset + rb.Position();
		rb.setPosition(position.x, position.y);
		circleCollider2D->m_body = rb.m_body;
		b2FixtureDef fixtureDef;
		fixtureDef.shape = &circleShape;
		fixtureDef.density = circleCollider2D->Density;
		fixtureDef.friction = circleCollider2D->Friction;
		rb.m_body->CreateFixture(&fixtureDef);
	}


	void Physics2DWorld::SetGravity(const glm::vec2& newGravity)
	{
		gravity = { newGravity.x, newGravity.y };
		world->SetGravity(gravity);
	}


	const glm::vec2 Physics2DWorld::Gravity()
	{
		return { gravity.x, gravity.y };
	}

	void Physics2DWorld::Reset()
	{
		for (b2Body* b : bodies)
		{
			world->DestroyBody(b);
		}
		bodies.clear();
		s_id.clear();
	}


	void Physics2DWorld::ImGuiEditor()
	{
		EditorUI::BeginWindow("Physics 2D World");
		EditorUI::BeginPropertyGrid();

		glm::vec2 g = { gravity.x, gravity.y };
		if (EditorUI::Property("Gravity", g))
		{
			SetGravity({ g.x, g.y });
		}
		gravity = { g.x, g.y };

		EditorUI::EndPropertyGrid();
		EditorUI::EndWindow();
	}

	b2World& Physics2DWorld::GetWorld()
	{
		return *world;
	}

	void Physics2DWorld::Step(float dt)
	{
		float timeStep = 1.0f / 60.0f;
		world->Step(timeStep, 6, 2);
	}


	void Physics2DWorld::DebugDraw()
	{
		if(s_DrawDebug)	world->DebugDraw();
	}

	Physics2DWorld* Physics2D::getWorld()
	{
		if (s_World == nullptr)
		{
			s_World = new Physics2DWorld();
		}
		return s_World;
	}
