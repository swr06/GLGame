#pragma once

#include <GL/glew.h>

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Core\OpenGL Classes\Index Buffer\IndexBuffer.h"
#include "Core\OpenGL Classes\Vertex Array\VertexArray.h"
#include "Core\OpenGL Classes\Vertex Buffer\VertexBuffer.h"
#include "Core\OpenGL Classes\Texture\Texture.h"
#include "Core\OpenGL Classes\Shader\Shader.h"
#include "Core\OpenGL Classes\GLDebug\GLDebug.h"
#include "Core\Engine\Macros\Macros.h"
#include "Core\Engine\Lighting\Lighting.h"

namespace GLGame
{
	void DrawLight(Light light);

}