#include "ray.h"

	Ray::Ray(glm::vec2 Position, glm::vec2 Direction)
	{

	}

	Ray::Ray(glm::vec2 Position, float Angle)
		: m_Position(Position)
	{
		m_Direction = glm::vec2((float)glm::cos(Angle), (float)glm::sin(Angle));
	}

	bool Ray::Check(const std::vector<Box2D>& ToCheck, std::vector<HitData>& Hits) const
	{
		float max = INFINITY;
		for (const Box2D& Box : ToCheck)
		{
			glm::vec2 HitLocation;
			if (Check(Box, HitLocation))
			{
				float d = glm::distance2(m_Position, HitLocation);
				if (d < max)
				{
					max = d;
					HitData Hit;
					Hit.Box = Box;
					Hit.Location = HitLocation;
					Hits.push_back(Hit);
				}
			}
		}

		return Hits.size() > 0;
	}

	bool Ray::Check(const Box2D& ToCheck, glm::vec2& HitLocation) const
	{
		if (ToCheck.Contains(m_Position))
		{
			//bottom
			if (Check(ToCheck.BottomLeft(), ToCheck.BottomRight(), HitLocation))
				return true;
			//left
			if (Check(ToCheck.BottomLeft(), ToCheck.TopLeft(), HitLocation))
				return true;
			//top
			if (Check(ToCheck.TopLeft(), ToCheck.TopRight(), HitLocation))
				return true;

			//Right
			if (Check(ToCheck.TopRight(), ToCheck.BottomRight(), HitLocation))
				return true;
		}

		bool checkTop = (m_Position.y > m_Position.y + ToCheck.m_Bounds.y);
		bool checkBot = (m_Position.y < ToCheck.m_Position.y);
		bool checkLeft = (m_Position.x < ToCheck.m_Position.x);
		bool checkRight = (m_Position.x > ToCheck.m_Position.x + ToCheck.m_Bounds.x);

		if (checkTop)
		{
			if (m_Direction.y > 0) 
				return false;

			//top
			if (Check(ToCheck.TopLeft(), ToCheck.TopRight(), HitLocation))
				return true;
		}

		if (checkBot)
		{
			if (m_Direction.y < 0)
				return false;

			//bottom
			if (Check(ToCheck.BottomLeft(), ToCheck.BottomRight(), HitLocation))
				return true;
		}

		if (checkRight)
		{
			if (m_Direction.x > 0)
				return false;

			//Right
			if (Check(ToCheck.TopRight(), ToCheck.BottomRight(), HitLocation))
				return true;
		}

		if (checkLeft)
		{
			if (m_Direction.x < 0)
				return false;

			//Left
			if (Check(ToCheck.BottomLeft(), ToCheck.TopLeft(), HitLocation))
				return true;;
		}


		return false;
	}

	bool Ray::Check(const glm::vec2& start, const glm::vec2& end, glm::vec2& HitLocation) const
	{
		const float x1 = start.x;
		const float y1 = start.y;

		const float x2 = end.x;
		const float y2 = end.y;

		const float x3 = m_Position.x;
		const float y3 = m_Position.y;

		const float x4 = m_Position.x + m_Direction.x;
		const float y4 = m_Position.y + m_Direction.y;

		const float den = (x1 - x2) * (y3 - y4) - (y1 - y2) * (x3 - x4);

		const float t = ((x1 - x3) * (y3 - y4) - (y1 - y3) * (x3 - x4)) / den;
		const float u = -((x1 - x2) * (y1 - y3) - (y1 - y2) * (x1 - x3)) / den;
		
		if (den == 0)
			return false;

		if (t > 0 && t < 1 && u > 0)
		{
			HitLocation = glm::vec2(x1 + t * (x2 - x1), y1 + t * (y2 - y1));
			return true;
		}

		return false;
	}

	void Ray::LookAt(const glm::vec2& Target)
	{
		m_Direction.x = Target.x - m_Position.x;
		m_Direction.y = Target.y - m_Position.y;
		m_Direction = glm::normalize(m_Direction);
	}

	Sensor::Sensor(const glm::vec2& Position, const glm::vec2& Direction, float FOV, float separation)
		: m_Position(Position), m_Direction(Direction), m_Angle(FOV), m_Separation(separation)
	{
		for (float i = 0; i < FOV; i += separation)
		{
			m_Rays.push_back(Ray(m_Position, glm::radians(i)));
		}
	}

	void Sensor::Translate(const glm::vec2& Position)
	{
		m_Position = Position;
		for (Ray& r : m_Rays)
		{
			r.m_Position = m_Position;
		}
	}

	void Sensor::LookAt(const glm::vec2& Target)
	{
		m_Direction.x = Target.x - m_Position.x;
		m_Direction.y = Target.y - m_Position.y;
		m_Direction = glm::normalize(m_Direction);
	}

	bool Sensor::Check(const std::vector<Box2D>& ToCheck, std::vector<HitData>& Hits)
	{
		for (const Ray& r : m_Rays)
		{
			std::vector<HitData> TempHits;
			r.Check(ToCheck, TempHits);
			Hits.insert(Hits.end(), TempHits.begin(), TempHits.end());
		}

		return Hits.size() > 0;
	}