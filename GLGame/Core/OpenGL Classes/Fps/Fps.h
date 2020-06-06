#pragma once

#include "Core\OpenGL Classes\GLDebug\GLDebug.h"
#include <GLFW/glfw3.h> // For glfwGetTime()

#include <windows.h> 
#include <sstream>

namespace GLGame
{
	void CalculateFrameRate(GLFWwindow* pWindow);
	long long int GetFPSCount();
	void DisplayFrameRate(GLFWwindow* pWindow, const std::string& title);
	void CalculateFrameRateDeprecated();
}