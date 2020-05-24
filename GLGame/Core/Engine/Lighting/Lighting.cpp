#include "Lighting.h"

namespace GLGame
{
	Light::Light(const glm::vec3& pos, const glm::vec4& color, float diameter)
	{
		m_Position = pos;
		m_Color = color;
		m_Diameter = diameter;
	}

	BlinkingLight::BlinkingLight(Light light, int speed, int frames, float max_brightness) : m_Speed(1), m_MaxBrightness(1.0f), m_CurrentFrame(light)
	{
		m_CurrFrameNo = 0;
		m_Speed = speed;
		m_MaxBrightness = max_brightness;

		Light _light = light;

		for (int i = 0; i < frames; i++)
		{
			_light.m_Color.a = max_brightness / i;
			m_Frames.push_back(_light);
		}
	}

	const Light& BlinkingLight::GetCurrentLightFrame()
	{
		return m_CurrentFrame;
	}

	void BlinkingLight::UpdateLightPulse(long long CurrFrame)
	{
		if (CurrFrame % m_Speed == 0)
		{
			if (m_CurrFrameNo == m_Frames.size() || m_CurrFrameNo > m_Frames.size())
			{
				m_CurrFrameNo = 0;
			}

			m_CurrentFrame = m_Frames[m_CurrFrameNo];
			m_CurrFrameNo++;
		}
	}
}