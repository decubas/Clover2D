#pragma once
#include "Entities/Minigame.h"
#include "Entities/Card.h"

class CoolingMinigame : public Minigame
{
public:
	CoolingMinigame();

	void OnUpdate(float fDeltaTime) override;
	void OnDraw() override;
	void OnMouseDown(int button);
	void OnMouseReleased(int button);



	float m_fTemperature = 0.f;
	ProgressBar Bar;
	Button m_Button;
	glm::vec2 ButtonOffset;
	glm::vec2 BarOffset;
	Ref<SubTexture2D> m_ButtonTexture;
	Ref<SubTexture2D> m_ButtonPressedTexture;

};

