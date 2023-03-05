#pragma once


#include <Layers/layer.h>
#include <scene/scene_manager.h>
#include "physics2d_world.h"



	class Physics2DLayer : public Layer 
	{
	public:
		Physics2DWorld* world;
		bool isPlaying = false;

		void OnUpdate(float delta) override
		{
			if (!isPlaying) return;

			world->Step(delta);
		}

		void RenderDebug()
		{
			world->DebugDraw();
		}

		void OnAttach() override
		{
			world = Physics2D::getWorld();
		}

		void Play()
		{
			isPlaying = true;
		}

		void Stop()
		{
			isPlaying = false;
		}

	};
