#include "Helper.h"

namespace GLGame
{
	void ToRGBA(glm::vec4& color) 
	{
		color.r = 1.0f / color.r;
		color.g = 1.0f / color.g;
		color.b = 1.0f / color.b;
		color.a = 1.0f / color.a;
	}
}