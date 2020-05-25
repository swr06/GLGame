#include "Lighting.h"

namespace GLGame
{
	Light::Light(const glm::vec3& pos, const glm::vec4& color, float sz)
	{
		m_Position = pos;
		m_Color = color;
		m_Size = sz;
	}

	BlinkingLight::BlinkingLight(Light light, int speed, int frames, float max_brightness) : m_Speed(1), m_MaxBrightness(1.0f), m_CurrentFrame(light)
	{
		m_CurrFrameNo = 0;
		m_Speed = speed;
		m_MaxBrightness = max_brightness;

		Light _light = light;

		for (int i = 1; i < frames; i++)
		{
			_light.m_Color.a = max_brightness / i;
			m_Frames.push_back(_light);
		}
	}

	const Light& BlinkingLight::GetCurrentLightFrame()
	{
		return m_CurrentFrame;
	}

	void BlinkingLight::UpdateLightBlink(long long CurrFrame)
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

	// Pulsating light 

	PulsatingLight::PulsatingLight(Light light, int speed, int frames, float max_brightness) : m_Speed(1), m_MaxBrightness(1.0f), m_CurrentFrame(light)
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


		int size = m_Frames.size() - 1;

		for (int j = size; j >= 1; j--)
		{
			m_Frames.push_back(m_Frames[j]);
		}
	}

	const Light& PulsatingLight::GetCurrentLightFrame()
	{
		return m_CurrentFrame;
	}

	void PulsatingLight::UpdateLightPulse(long long CurrFrame)
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

	// Pulsating light radius
	PulsatingLightRadius::PulsatingLightRadius(Light light, int speed, int frames, float max_brightness) : m_Speed(1), m_MaxBrightness(1.0f), m_CurrentFrame(light)
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


		int size = m_Frames.size() - 1;

		for (int j = size; j >= 1; j--)
		{
			m_Frames.push_back(m_Frames[j]);
		}
	}

	const Light& PulsatingLightRadius::GetCurrentLightFrame()
	{
		return m_CurrentFrame;
	}

	void PulsatingLightRadius::UpdateLightPulse(long long CurrFrame)
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