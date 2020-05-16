#include "AABB.h"

namespace GLGame
{
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

	bool CheckAABBCollision(AABB one, AABB two)
	{
		bool collisionX = one.x + one.w >= two.x && two.x + two.w >= one.x;
		bool collisionY = one.y + one.h >= two.y && two.y + two.h >= one.y;

		return collisionX && collisionY;
	}
}