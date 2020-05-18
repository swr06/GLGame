#pragma once

#include "Core\OpenGL Classes\GLDebug\GLDebug.h"

namespace GLGame
{
	

	struct AABBCollisionDir
	{
		float UP_POS;
		float DOWN_POS;
		float LEFT_POS;
		float RIGHT_POS;
	};

	struct CircleCollisionDir
	{
		float Radius;
		float pos;
	};

	CollisionDirection GetCollisionDirection(AABBCollisionDir AABB_Collision_dir)
	{
		
	}
}