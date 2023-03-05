
#include "rectangle.h"
#include "Graphics/renderer2D.h"


	Box2D::Box2D(float x, float y, float Width, float Height)
	{
		m_Position = { x, y };
		m_Bounds = { Width, Height };
	}

	Box2D::Box2D(const glm::vec2& Position, const glm::vec2& Bounds)
		: m_Position(Position), m_Bounds(Bounds)
	{}

	void Box2D::DrawDebug()
	{
		glm::vec4 BoxColor = { 0, 0, 1, 1 };
		Renderer2D::DrawLine(TopLeft(), TopRight(), 2.f, BoxColor);
		Renderer2D::DrawLine(TopRight(), BottomRight(), 2.f, BoxColor);
		Renderer2D::DrawLine(BottomRight(), BottomLeft(), 2.f, BoxColor);
		Renderer2D::DrawLine(BottomLeft(), TopLeft(), 2.f, BoxColor);
		BoxColor = { 0, 1, 0, 1 };
		Renderer2D::DrawQuad(m_Position, { 10, 10 }, BoxColor);
		BoxColor = { 1, 0, 0, 1 };
		Renderer2D::DrawQuad(Center(), { 10, 10 }, BoxColor);
	}

	bool Box2D::Contains(const glm::vec2& Point) const
	{
		return (Point.x >= m_Position.x &&
			Point.x <= m_Position.x + m_Bounds.x &&
			Point.y >= m_Position.y &&
			Point.y <= m_Position.y + m_Bounds.y);
	}

	bool Box2D::Intersects(const Box2D& Other) const
	{
		return
			(Contains(Other.BottomLeft()) ||
				Contains(Other.BottomRight()) ||
				Contains(Other.TopRight()) ||
				Contains(Other.TopLeft()) ||
				Other.Contains(TopLeft()) ||
				Other.Contains(BottomLeft()) ||
				Other.Contains(TopRight()) ||
				Other.Contains(BottomRight()));
	}

	glm::vec2 Box2D::Center() const
	{
		return m_Position + m_Bounds * 0.5f;
	}

	glm::vec2 Box2D::TopRight() const
	{
		return m_Position + m_Bounds;
	}

	glm::vec2 Box2D::TopLeft() const
	{
		return m_Position + glm::vec2{ 0.0f, m_Bounds.y };
	}

	glm::vec2 Box2D::BottomRight() const
	{
		return m_Position + glm::vec2{ m_Bounds.x, 0.0f };
	}

	glm::vec2 Box2D::BottomLeft() const
	{
		return m_Position;
	}

	bool Box2D::operator==(const Box2D& b) const
	{
		return m_Position == b.m_Position && m_Bounds == b.m_Bounds;
	}

	bool Box2D::operator!=(const Box2D& b) const
	{
		return *this != b;
	}

