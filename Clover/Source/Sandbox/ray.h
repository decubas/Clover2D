#ifndef RAY_H_
#define RAY_H_ 1

#include <Clover.h>

#include "rectangle.h"

struct HitData
{
	glm::vec2 Location;
	Box2D Box;
};

class Ray
{
public:

	Ray(glm::vec2 Position, glm::vec2 Direction);
	Ray(glm::vec2 Position, float Angle);

	void LookAt(const glm::vec2& Target);

	bool Check(const std::vector<Box2D>& ToCheck, std::vector<HitData>& Hits) const;
	bool Check(const Box2D& ToCheck, glm::vec2& HitLocation) const;
	bool Check(const glm::vec2& start, const glm::vec2& end, glm::vec2& HitLocation) const;
public:
	glm::vec2 m_Position = glm::vec2(0.0f, 0.0f);
	glm::vec2 m_Direction = glm::vec2(0.0f, 0.0f);
private:

};


class Sensor
{
public:
	Sensor(const glm::vec2& Position, const glm::vec2& Direction, float FOV, float separation = 1.0f);
	void Translate(const glm::vec2& Position);
	void LookAt(const glm::vec2& Target);
	bool Check(const std::vector<Box2D>& ToCheck, std::vector<HitData>& Hits);

	std::vector <Ray> m_Rays;
	glm::vec2 m_Position;
	glm::vec2 m_Direction;
	float m_Angle = 360.0f;
	float m_Separation = 1.0f;

};
#endif