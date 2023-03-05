#include "Prot_Camera.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

#include <Graphics/camera.h>
#include <Graphics/window.h>
#include <Delegate/delegate.h>


PrototypeCamera::PrototypeCamera(float size, float farClip, float nearClip)
	: m_Size(size), m_NearClip(nearClip), m_FarClip(farClip)
{
	m_Size = size;
	m_FarClip = farClip;
	m_NearClip = nearClip;
	RecalculateProjection();
	m_Transform.m_Position = glm::vec3(0, 0, 0);
	m_Transform.m_Scale = glm::vec3(1.0f);
	m_Transform.m_Rotation = glm::vec3(0.0f);
}
float PrototypeCamera::GetRotationSpeed()
{
	return 0.3f;
}
void PrototypeCamera::OnUpdate(float ts, float speed)
{
	bool moved = false;

	constexpr glm::vec3 upDirection(0.0f, 1.0f, 0.0f);
	constexpr glm::vec3 rightDirection(-1.0f, 0.0f, 0.0f);

	// Movement
	if (Input::isKeyPressed(Key::W))
	{
		m_Transform.m_Position += upDirection * speed * ts;
		moved = true;
	}
	else if (Input::isKeyPressed(Key::S))
	{
		m_Transform.m_Position -= upDirection * speed * ts;
		moved = true;
	}
	if (Input::isKeyPressed(Key::A))
	{
		m_Transform.m_Position -= rightDirection * speed * ts;
		moved = true;
	}
	else if (Input::isKeyPressed(Key::D))
	{
		m_Transform.m_Position += rightDirection * speed * ts;
		moved = true;
	}

	if (moved)
	{
		RecalculateProjection();
	}
}

void PrototypeCamera::OnResize(float width, float height)
{
	if (width == m_ViewportWidth && height == m_ViewportHeight)
		return;
	m_AspecRatio = width / height;
	m_ViewportWidth = width;
	m_ViewportHeight = height;

	RecalculateProjection();
}

void PrototypeCamera::RecalculateProjection()
{
	float right = 0.5f * m_AspecRatio * m_Size;
	float top = 0.5f * m_Size;

	m_Projection = glm::ortho(-right, right, top, -top, m_NearClip, m_FarClip);
}