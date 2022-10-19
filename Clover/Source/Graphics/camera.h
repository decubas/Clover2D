#ifndef __CAMERA_H__
#define __CAMERA_H__ 1

#include <Clover.h>

#include "framebuffer.h"
#include <Delegate/delegate.h>


struct Transform
{
	Transform(CVec3 Position, CVec3 Rotation, CVec3 Size)
	{
		CMat4 rotation = glm::toMat4(glm::quat(Rotation));

		matrix = glm::translate(CMat4(1.0f), Position) * rotation * glm::scale(CMat4(1.0f), Size);

	}

	void GetTransformDecomposition(CVec3& Position, glm::quat& Rotation, CVec3& Scale)
	{
		CVec3 skew;
		CVec4 perspective;
		glm::decompose(matrix, Scale, Rotation, Position, skew, perspective);
	}

	CMat4 matrix;
};

class TransformComponent
{
public:
	CVec3 m_Position;
	CVec3 m_Scale;
	CVec3 m_Rotation;

	TransformComponent();
	~TransformComponent() {};


	void ResetTransform();

	CMat4& RecalculateModel()
	{
		CMat4 rotation = glm::toMat4(glm::quat(m_Rotation));

		m_AbsoluteModelMatrix = glm::translate(CMat4(1.0f), m_Position) * rotation * glm::scale(CMat4(1.0f), m_Scale);

		return m_AbsoluteModelMatrix;
	}

	CMat4& getTransformMatrix()
	{
		return m_AbsoluteModelMatrix;
	};

	void setAbsoluteModelMatrix(const CMat4& cumulative);

protected:
	CMat4 m_AbsoluteModelMatrix;
	CMat4 m_LocalModelMatrix;
};

	class Camera : public Cl_Object
	{
	public:
		Camera() = default;
		Camera(const CMat4& projection)
			: m_Projection(projection) {}

		virtual ~Camera() = default;

		void SetProjection(CMat4& projection) { m_Projection = projection; }
		const CMat4& GetProjection() const { return m_Projection; }
		virtual CVec4 ScreenToWorld(const CVec2& Position);

	protected:
		CMat4 m_Projection = CMat4(1.0f);
	};

	class SceneCamera : public Camera
	{
	public:
		enum class ProjectionType { Perspective = 0, Orthographic = 1 };
	public:
		SceneCamera();
		virtual ~SceneCamera() = default;

		void SetPerspective(float FOV, float farClip, float nearClip);
		void SetOrthographic(float size, float farClip, float nearClip);

		void SetViewportSize(u32 width, u32 height);

		ProjectionType GetProjectionType() { return m_ProjectionType; }
		void SetProjectionType(ProjectionType type) { m_ProjectionType = type; }

		void SetPerspectiveFOV(float FOV) { m_PerspFOV = FOV; }
		void SetPerspectiveNear(float Near) { m_PerspNear = Near; }
		void SetPerspectiveFar(float Far) { m_PerspFar = Far; }

		void SetOrthographicSize(float Size) { m_OrthoSize = Size; }
		void SetOrthographicNear(float Near) { m_OrthoNear = Near; }
		void SetOrthographicFar(float Far) { m_OrthoFar = Far; }

		float GetOrthographicSize() { return m_OrthoSize; }
		float GetOrthographicFar() { return m_OrthoFar; }
		float GetOrthographicNear() { return m_OrthoNear; }

		float GetPerspectiveFOV() { return glm::degrees(m_PerspFOV); }
		float GetPerspectiveFar() { return m_PerspFar; }
		float GetPerspectiveNear() { return m_PerspNear; }
	protected:
		void RecalculateProjection();
	protected:
		ProjectionType m_ProjectionType = ProjectionType::Orthographic;
		float m_OrthoSize = 10.0f;
		float m_OrthoNear = -1.0f, m_OrthoFar = 1.0f;

		CVec2 m_Viewport;

		float m_PerspFOV = glm::radians(45.0f);
		float m_PerspNear = 0.01f, m_PerspFar = 1000.0f;

		float m_AspecRatio = 0.0f;
	};
	
	class EditorCamera : public SceneCamera
	{
	public:
		EditorCamera();
		virtual ~EditorCamera() = default;

		void OnUpdate(float delta, float PanSpeed = 1.f, float ZoomSpeed = 1.f);

		TransformComponent& GetTransform() { return m_Transform; }
		virtual CVec4 ScreenToWorld(const CVec2& Position) override;

	private:

		void MousePan(const CVec2& delta);
		void MouseZoom(float delta);

	private:

		TransformComponent m_Transform;
		CVec2 m_InitialMousePosition = { 0.0f, 0.0f };
	};

#endif
