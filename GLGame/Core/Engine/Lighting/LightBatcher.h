#pragma once

#include <thread>
#include <GL/glew.h>

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Core\OpenGL Classes\GLDebug\GLDebug.h"
#include "Core\Engine\Lighting\Lighting.h"

namespace GLGame
{
	void DrawLight(Light light);
}