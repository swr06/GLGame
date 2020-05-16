#pragma once

#include <iostream>
#include <string>
#include <vector>
#include "Core\OpenGL Classes\GLDebug\GLDebug.h"

namespace GLGame
{
	class Particle
	{
		glm::vec4 StartColor;
		glm::vec4 EndColor;
		float LifeSpan;
	};

	class ParticleEmitter
}