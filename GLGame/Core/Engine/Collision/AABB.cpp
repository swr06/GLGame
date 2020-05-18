#include "AABB.h"

namespace GLGame
{
	// Experimental collision method

	/*bool CheckAABBCollision(AABB c1, AABB c2)
	{
		glm::vec2 ScreenDimensions = glm::vec2(c1.w, c1.h);

		const float MinimumDistanceX = c2.w / 2.0f + ScreenDimensions.x;
		const float MinimumDistanceY = c2.h / 2.0f + ScreenDimensions.y;

		glm::vec2 Pos, Dimensions;

		Pos.x = c2.x;
		Pos.y = c2.y;
		Dimensions.x = c2.w;
		Dimensions.y = c2.h;

		glm::vec2 CenterPosition = Pos + Dimensions / 2.0f;
		glm::vec2 CenterCameraPosition;

		CenterCameraPosition.x = (int) c1.w / 2;
		CenterCameraPosition.y = (int) c1.y / 2;

		glm::vec2 Distance = CenterPosition - CenterCameraPosition;

		float xDepth = MinimumDistanceX - abs(Distance.x);
		float yDepth = MinimumDistanceY - abs(Distance.y);

		if (xDepth > 0 && yDepth > 0)
		{
			return true;
		}

		else
		{
			return false;
		}

		return false;
	}*/


	bool ObjectInCameraView(AABB Camera, AABB Object)
	{
		// Same as AABB collision method but it has an error factor

		bool collisionX = Camera.x + Camera.w >= Object.x && Object.x + Object.w >= Camera.x;
		bool collisionY = Camera.y + Camera.h >= Object.y && Object.y + Object.h >= Camera.y;

		return collisionX && collisionY;
	}

	bool CheckAABBCollision(AABB one, AABB two, AABBCollisionMask* mask)
	{
		float one_width = one.x + one.w;
		float one_height = one.y + one.h;
		float two_width = two.x + two.w;
		float two_height = two.y + two.h;

		bool collisionX = one_width >= two.x && two_width >= one.x;
		bool collisionY = one_height >= two.y && two_height >= one.y;

		if (collisionX && collisionY)
		{
			// These was a collision on both the axes.

			//mask->CheckPosition();
			if (one.x < two.x)
			{
				std::cout << "\nThe right side of square 1 collided with the left side of square 2";
			}

			if (one.x > two.x)
			{
				std::cout << "\nThe left side of square 1 collided with the right side of square 2";
			}

			if (one.y < two.y)
			{
				std::cout << "\nThe bottom side of square 1 collided with the top side of square 2";
			}

			if (one.y > two.y)
			{
				std::cout << "\nThe top side of square 1 collided with the bottom side of square 2";
			}

			return true;
		}

		else
		{
			return false;
		}
	}
}
