#pragma once

#include <iostream>
#include <string>
#include <vector>

#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Core\OpenGL Classes\GLDebug\GLDebug.h"

namespace GLGame
{
	struct Particle
	{
		glm::vec3 Pos;
		glm::vec4 StartColor;
		glm::vec4 EndColor;
		float Velocity;
		float LifeSpan;
	};

	class ParticleEmitter
	{
	public : 

		void Emit()
		{

		}
	};
}