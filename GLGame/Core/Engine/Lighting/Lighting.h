#pragma once

#include <GL/glew.h>

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <vector>

#include "Core\OpenGL Classes\GLDebug\GLDebug.h"

namespace GLGame
{
	class Light
	{
	public : 

		// Since this class only has 3 variables that need to be changed, there's no practical use of making them private.
		Light(const glm::vec3& pos, const glm::vec4& color, float sz);

		glm::vec3 m_Position;
		glm::vec4 m_Color;
		float m_Size;
	};

	class BlinkingLight
	{
	public : 
		BlinkingLight(Light light, int speed, int frames, float max_brightness);
		const Light& GetCurrentLightFrame();
		void UpdateLightBlink(long long CurrFrame);

	private : 

		std::vector<Light> m_Frames;
		Light m_CurrentFrame;
		int m_Speed;
		int m_CurrFrameNo;
		float m_MaxBrightness;
	};

	class PulsatingLight
	{
	public:
		PulsatingLight(Light light, int speed, int frames, float max_brightness);
		const Light& GetCurrentLightFrame();
		void UpdateLightPulse(long long CurrFrame);

	private:

		std::vector<Light> m_Frames;
		Light m_CurrentFrame;
		int m_Speed;
		int m_CurrFrameNo;
		float m_MaxBrightness;
	};

	class PulsatingLightRadius
	{
	public:
		PulsatingLightRadius(Light light, int speed, int frames, float max_brightness);
		const Light& GetCurrentLightFrame();
		void UpdateLightPulse(long long CurrFrame);

	private:

		std::vector<Light> m_Frames;
		Light m_CurrentFrame;
		int m_Speed;
		int m_CurrFrameNo;
		float m_MaxBrightness;
	};
}