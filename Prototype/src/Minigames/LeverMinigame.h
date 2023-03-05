#pragma once
#include "Entities/Minigame.h"
#include "Entities/Card.h"

class LeverDraggable : public Draggable
{
public:

	void OnMouseDragged(const glm::vec2& MousePos) override;
	void OnDraw();

	glm::vec2 gfxPosition;
	float Offset;
	float Min = 0.f;
	float Max;

	float RecoverySpeed = 0.05f;
	float Speed = 0.005f;
};

class LeverMinigame : public Minigame
{
public:
	LeverMinigame();

	void OnUpdate(float fDeltaTime) override;
	void OnDraw() override;

	LeverDraggable Lever;
	glm::vec2 LeverOffset;

};


