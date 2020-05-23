#pragma once

#include <thread>
#include <GL/glew.h>

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Core\OpenGL Classes\GLDebug\GLDebug.h"

namespace GLGame
{
	class Light
	{
	public : 

		Light(const glm::vec3& pos, const glm::vec4& color, float diameter);

		glm::vec3 m_Position;
		glm::vec4 m_Color;
		float m_Diameter;

	};
}