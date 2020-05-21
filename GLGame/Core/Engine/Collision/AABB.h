#pragma once

#include <iostream>
#include <stack>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Core\OpenGL Classes\GLDebug\GLDebug.h"

namespace GLGame
{
	struct AABB
	{
		float x;
		float y;
		float w;
		float h;
	};

	struct Point
	{
		Point(float _x, float _y)
		{
			x = _x;
			y = _y;
		}

		Point()
		{
			
		}

		float x;
		float y;
	};

	class AABBCollisionMask
	{
	public : 

		AABBCollisionMask() : m_SizeOfThreshold(20), m_CurrentWriteElement(0), m_OldestElement(0), m_EntireBufferWrittenTo(false), m_XThreshold(0), m_YThreshold(0)
		{
			Point p = {-1.0f, -1.0f};

			m_Points = new Point[m_SizeOfThreshold];
			memset(m_Points, -1, m_SizeOfThreshold * sizeof(Point));
		}

		void CheckAABBCollision(AABBCollisionMask m2)
		{

		}

		void WritePointToBuff(Point p)
		{
			if (m_CurrentWriteElement == (m_SizeOfThreshold - 1))
			{
				m_EntireBufferWrittenTo = true;
				m_CurrentWriteElement = 0;
			}

			m_Points[m_CurrentWriteElement] = p;
			m_CurrentWriteElement += 1;
		}

		void CheckPosition()
		{
			Point delta;
			Point point;
			Point oldest_point;

			// Get the last written point
			point = m_Points[m_CurrentWriteElement - 1];

			if (m_EntireBufferWrittenTo)
			{
				oldest_point = m_Points[m_CurrentWriteElement];
			}

			else
			{
				oldest_point = m_Points[0];
			}

			delta.x = point.x - oldest_point.x;
			delta.y = point.y - oldest_point.y;

			if (abs(delta.x) >= m_XThreshold)
			{
				if (delta.x < 0)
				{
					std::cout << "RIGHT !";
				}

				else if (delta.x > 0)
				{
					std::cout << "LEFT !";
				}
			}

			if (abs(delta.y) >= m_XThreshold)
			{
				if (delta.y < 0)
				{
					std::cout << "UP !";
				}

				else if (delta.y > 0)
				{
					std::cout << "DOWN !";
				}
			}
		}

	private:
		
		const int m_SizeOfThreshold;
		const float m_XThreshold;
		const float m_YThreshold;
		int m_CurrentWriteElement;
		int m_OldestElement;
		bool m_EntireBufferWrittenTo;
		Point* m_Points;

	};

	enum CollisionDirection
	{
		UP = 0,
		DOWN,
		LEFT,
		RIGHT
	};

	struct CircleCollision {};

	bool CheckAABBCollision(AABB one, AABB two);
	bool ObjectInCameraView(AABB Camera, AABB Object);
	
}