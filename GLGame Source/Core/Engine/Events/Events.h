#pragma once

#define GLEW_STATIC

#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>
#include <GL/glew.h>

#include "Core\OpenGL Classes\GLDebug\GLDebug.h"

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Core\Engine\Macros\Macros.h"

namespace GLGame
{
	struct Event;
	struct _PathDrop
	{
		char** paths = nullptr;
		int count = 0;
	};

	namespace GameInternal
	{
		void _IntAddEventToQueue(Event e); 
	}

	enum Event_Type
	{
		Event_KeyPressed = 0,
		Event_KeyReleased,
		Event_Key,
		Event_MouseButtonPressed,
		Event_CursorMoved,
		Event_CursorEnter, // When the cursor enters the window
		Event_MouseButtonReleased,
		Event_MouseScrollWheel,
		Event_FrameBufferResize,
		Event_WindowResize,
		Event_WindowClose,
		Event_WindowPosition,
		Event_PathDrop,

		Event_WindowIconified,
		Event_WindowMaximized,
		Event_WindowInputFocus,

		Event_ControllerA,
		Event_ControllerB,
		Event_ControllerC,
		Event_ControllerD,
		Event_Collision,
		Event_Unknown
	};

	struct Event
	{
		double MouseX = 0;
		double MouseY = 0;
		double MouseScrollOffsetX = 0;
		double MouseScrollOffsetY = 0;
		double Time;
		double KeyHoldTime;
		int ObjectID_1 = 0;
		int ObjectID_2 = 0;
		int KeyCode = 0;
		int MouseButton = 0;
		int Cursor = 0;
		int WindowPositionX = 0; 
		int WindowPositionY = 0; 
		int WindowSizeCoordsX = 0;
		int WindowSizeCoordsY = 0;
		int WindowFrameBufferX = 0;
		int WindowFrameBufferY = 0;
		bool CursorEntered = 0;
		bool WindowIconified = 0;
		bool WindowVisible = 0;
		bool WindowHasFocus = 0;
		bool WindowIsMaximized = 0;
		bool WindowIsHovered = 0;
		bool WindowIsFloating = 0;
		bool WindowToClose = 0;
		Event_Type EventType ;
		_PathDrop PathDrops;
		std::string WindowCaption;
	};
	
	void GLGameEventInit();
	void GLGameSetDefaults(Event& e, GLFWwindow* window);
	void GLGameKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
	void GLGameWindowSizeCallback(GLFWwindow* window, int width, int height);
	void GLGameMouseCallback(GLFWwindow* window, int button, int action, int mods);
	void GLGameScrollCallback(GLFWwindow* window, double xoffset, double yoffset);
	void GLGameDropCallback(GLFWwindow* window, int count, const char** paths);
	void GLGameCursorPosCallback(GLFWwindow* window, double xpos, double ypos);
	void GLGameCursorEnterCallback(GLFWwindow* window, int entered);
	void GLGameWindowCloseCallback(GLFWwindow* window);
	void GLGameFramebufferSizeCallback(GLFWwindow* window, int width, int height);
	void GLGameWindowPositionCallback(GLFWwindow* window, int xpos, int ypos);
	void GLGameWindowIconifiedCallback(GLFWwindow* window, int iconified);
	void GLGameWindowMaximizedCallback(GLFWwindow* window, int maximized);
	void GLGameWindowFocusCallback(GLFWwindow* window, int focused);
	bool* GLGameInternalGetKeyVal();
}