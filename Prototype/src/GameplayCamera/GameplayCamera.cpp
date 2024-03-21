#include "GameplayCamera.h"


GameplayCamera::GameplayCamera()
{
	TargetPosition = m_Transform.m_Position;
	TargetZoom = 100.f;
}

void GameplayCamera::Update(const float fDelta, const float Speed, const float ScrollSpeed, const float Smoothing)
{
	this->ScrollSpeed = ScrollSpeed;
	if (Input::isKeyPressed(Key::A))
	{
		TargetPosition.x += fDelta * Speed;
	}

	if (Input::isKeyPressed(Key::D))
	{
		TargetPosition.x -= fDelta * Speed;
	}

	if (Input::isKeyPressed(Key::W))
	{
		TargetPosition.y += fDelta * Speed;
	}

	if (Input::isKeyPressed(Key::S))
	{
		TargetPosition.y -= fDelta * Speed;
	}

	m_Transform.m_Position.x = dcMath::lerp(m_Transform.m_Position.x, TargetPosition.x, Smoothing);
	m_Transform.m_Position.y = dcMath::lerp(m_Transform.m_Position.y, TargetPosition.y, Smoothing);
	
	m_OrthoSize = dcMath::lerp(m_OrthoSize, TargetZoom, Smoothing);
	RecalculateProjection();
//	m_Transform.m_Scale.x = dcMath::lerp(m_Transform.m_Scale.x, TargetTransform.m_Scale.x, Smoothing);
//	m_Transform.m_Scale.y = dcMath::lerp(m_Transform.m_Scale.y, TargetTransform.m_Scale.y, Smoothing);
}

void GameplayCamera::OnScroll(float x, float y)
{
	if (TargetZoom + y < 50.f )
	{
		TargetZoom = 50.f;
		return;
	}

	TargetZoom += y * ScrollSpeed;
}

// CVec4 GameplayCamera::ScreenToWorld(const CVec2& Position)
// {
// 	return glm::inverse(m_Transform.RecalculateModel()) * (glm::inverse(m_Projection) * CVec4{ Position.x, Position.y, 0.f, 1.f });
// }

CVec2 GameplayCamera::GetScreenMousePos()
{
	return { Input::mouseX(), Input::mouseY() };
}

CVec2 GameplayCamera::GetWorldMousePos()
{
	return ScreenToWorld(Window::GetCurrentWindow()->MapToViewport({ Input::mouseX(), Input::mouseY() }));
}

void GameplayCamera::SetOrthographic(float size, float farClip, float nearClip)
{
	SceneCamera::SetOrthographic(size, farClip, nearClip);
	TargetZoom = size;
}

void GameplayCamera::RecalculateProjection()
{
	SceneCamera::RecalculateProjection();
}
