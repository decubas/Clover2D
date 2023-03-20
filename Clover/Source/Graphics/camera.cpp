#include <Clover.h>

#include <Graphics/camera.h>
#include <Graphics/window.h>
#include <Delegate/delegate.h>

TransformComponent::TransformComponent()
{
	ResetTransform();
}

void TransformComponent::setAbsoluteModelMatrix(const CMat4& cumulative)
{
	//m_AbsoluteModelMatrix = cumulative * getTransformMatrix();// *cumulative;
}

void TransformComponent::ResetTransform()
{
	m_LocalModelMatrix = CMat4(1.0f);
	m_Position = glm::vec3(0.0f);
	m_Rotation = glm::vec3(0.0f);
	m_Scale = glm::vec3(1.0f);
}

	EditorCamera::EditorCamera()
	{
	}

	void EditorCamera::OnUpdate(float fDeltaTime, float PanSpeed, float ZoomSpeed)
	{
		const glm::vec2& mouse{ Input::mouseX(), Input::mouseY() };
		glm::vec2 delta = (mouse - m_InitialMousePosition) * fDeltaTime;
		m_InitialMousePosition = mouse;
		
		if (Input::isKeyPressed(Key::LeftAlt))
		{
			if (Input::isMouseButtonPressed(Mouse::ButtonLeft))
				MousePan(delta * PanSpeed);
			else if (Input::isMouseButtonPressed(Mouse::ButtonRight))
				MouseZoom(delta.y * ZoomSpeed);
		}
	}

	void EditorCamera::MousePan(const CVec2& delta)
	{
		m_Transform.m_Position += glm::vec3(1, 0, 0) * delta.x;
		m_Transform.m_Position += glm::vec3(0, 1, 0) * delta.y;
	}

	void EditorCamera::MouseZoom(float delta)
	{
		if (m_Transform.m_Scale.x > delta)
		{
			m_Transform.m_Scale.x -= delta;
			m_Transform.m_Scale.y -= delta;
		}
	}

	CVec4 EditorCamera::ScreenToWorld(const CVec2& Position)
	{
		return CVec4{ Position.x, Position.y, 0.f, 1.f } * glm::inverse(m_Transform.getTransformMatrix() * m_Projection);
	}

	SceneCamera::SceneCamera()
	{
		RecalculateProjection();
	}

	void SceneCamera::SetPerspective(float FOV, float farClip, float nearClip)
	{
		m_ProjectionType = ProjectionType::Perspective;
		m_PerspFOV = glm::radians(FOV);
		m_PerspFar = farClip;
		m_PerspNear = nearClip;
		RecalculateProjection();
	}

	void SceneCamera::SetOrthographic(float size, float farClip, float nearClip)
	{
		m_ProjectionType = ProjectionType::Orthographic;
		m_OrthoSize = size;
		m_OrthoFar = farClip;
		m_OrthoNear = nearClip;
		RecalculateProjection();
	}

	void SceneCamera::SetViewportSize(u32 width, u32 height)
	{
		m_AspecRatio = (float)width / (float)height;
		m_Viewport = { width, height };
		RecalculateProjection();
	}

	void SceneCamera::RecalculateProjection()
	{
		//float right = 0.5f * m_AspecRatio * m_OrthoSize;
		//float top = 0.5f * m_OrthoSize;
		float right = m_OrthoSize;
		float top = m_OrthoSize;

		if (m_ProjectionType == ProjectionType::Orthographic)
			m_Projection = glm::ortho(-right, right, top, -top, m_OrthoNear, m_OrthoFar);
		else
			m_Projection = glm::perspective(m_PerspFOV, m_AspecRatio, m_PerspNear, m_PerspFar);
	}

	CVec4 Camera::ScreenToWorld(const CVec2& Position)
	{
		return CVec4{ Position.x, Position.y, 0.f, 1.f } * glm::inverse(m_Projection);
	}

	CVec4 Camera::WorldToScreen(const CVec2& Position)
	{
		return CVec4{ Position.x, Position.y, 0.f, 1.f } * (m_Projection);
	}
