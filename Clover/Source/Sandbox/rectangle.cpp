
#include "rectangle.h"


	Box2D::Box2D(float x, float y, float Width, float Height)
	{
		m_Position = { x, y };
		m_Bounds = { Width, Height };
	}

	Box2D::Box2D(const glm::vec2& Position, const glm::vec2& Bounds)
		: m_Position(Position), m_Bounds(Bounds)
	{}

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
			!(Other.m_Position.x - Other.m_Bounds.x >= m_Position.x + m_Bounds.x ||
			Other.m_Position.x + Other.m_Bounds.x <= m_Position.x - m_Bounds.x ||
			Other.m_Position.y - Other.m_Bounds.y >= m_Position.y + m_Bounds.y ||
			Other.m_Position.y + Other.m_Bounds.y <= m_Position.y - m_Bounds.y);
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

