#ifndef __BOX2D_H__
#define __BOX2D_H__ 1

#include <Clover.h>



class Box2D
{
public:
	Box2D() = default;
	~Box2D() = default;

	Box2D(const glm::vec2& Position, const glm::vec2& Bounds);
	Box2D(float x, float y, float Width, float Height);

	void DrawDebug();
	virtual bool Contains(const glm::vec2& Point) const;
	bool Intersects(const Box2D& Other) const;
	glm::vec2 Center() const;
	glm::vec2 TopRight() const;
	glm::vec2 TopLeft() const;
	glm::vec2 BottomRight() const;
	glm::vec2 BottomLeft() const;

	bool operator==(const Box2D& b) const;
	bool operator!=(const Box2D& b) const;

	glm::vec2 m_Position = {0, 0};
	glm::vec2 m_Bounds = { 1, 1 };
};

#endif
