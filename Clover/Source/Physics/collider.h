#pragma once

#include "collision.h"


class Collider2D : public Collision2DObject
{
public:
	glm::vec2 Offset = { 0.0f,0.0f };

	union
	{
		glm::vec2 Size = { 1.0f, 1.0f };
		float Radius;
	};

	float Density = 1.0f;
	float Friction = 0.3f;
	bool Trigger = false;
	
};
