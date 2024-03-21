#pragma once

#include <CloverEngine.h>
#include "Sandbox/rectangle.h"


class GameplayCamera : public SceneCamera
{
public:
	GameplayCamera();

	void Update(const float fDelta, const float Speed, const float ScrollSpeed, const float Smoothing = 0.5f);
	void OnScroll(float x, float y);
	

	
	CVec2 GetScreenMousePos();
	CVec2 GetWorldMousePos();
	
	virtual void SetOrthographic(float size, float farClip, float nearClip) override;
	virtual void RecalculateProjection() override;

protected:
	CVec2 TargetPosition;
	float TargetZoom;
	float ScrollSpeed;

};