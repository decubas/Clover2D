#pragma once

#include <Clover.h>
#include "Graphics/camera.h"

class PrototypeCamera : public Camera
{
public:
	PrototypeCamera(float size, float farClip, float nearClip);

	void OnUpdate(float ts, float speed);
	void OnResize(float width, float height);

	float GetRotationSpeed();
	const glm::vec3& GetPosition() const { return m_Transform.m_Position; }
	float GetSize() const { return m_Size; }

	TransformComponent m_Transform;
private:
	void RecalculateProjection();
private:

	float m_AspecRatio = 0.0f;
	float m_Size = 100.0f;
	float m_NearClip = 0.1f;
	float m_FarClip = 100.0f;

	glm::vec2 m_LastMousePosition{ 0.0f, 0.0f };
	glm::vec3 m_ForwardDirection{ 0.0f, 0.0f, 0.0f };

	uint32_t m_ViewportWidth = 0, m_ViewportHeight = 0;
};