#pragma once

#include <iostream>
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

	struct CircleCollision {};

	bool CheckAABBCollision(AABB one, AABB two);
	bool ObjectInCameraView(AABB Camera, AABB Object);
	
}