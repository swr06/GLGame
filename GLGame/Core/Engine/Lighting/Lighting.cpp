#include "Lighting.h"

namespace GLGame
{
	Light::Light(const glm::vec3& pos, const glm::vec4& color, float diameter)
	{
		m_Position = pos;
		m_Color = color;
		m_Diameter = diameter;
	}
}