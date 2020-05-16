#include "Events.h"

namespace GLGame
{
	bool* m_Keys;

	void GLGameSetDefaults(Event& e, GLFWwindow* window)
	{
		e.WindowIconified = glfwGetWindowAttrib(window, GLFW_ICONIFIED);
		e.WindowVisible = glfwGetWindowAttrib(window, GLFW_VISIBLE);
		e.WindowHasFocus = glfwGetWindowAttrib(window, GLFW_FOCUSED);
		glfwGetWindowPos(window, &e.WindowPositionX, &e.WindowPositionX);
		glfwGetWindowSize(window, &e.WindowSizeCoordsX, &e.WindowSizeCoordsY);
		glfwGetFramebufferSize(window, &e.WindowFrameBufferX, &e.WindowFrameBufferY);
		glfwGetCursorPos(window, &e.MouseX, &e.MouseX);
		e.WindowIsMaximized = glfwGetWindowAttrib(window, GLFW_MAXIMIZED);
		e.WindowIsHovered = glfwGetWindowAttrib(window, GLFW_HOVERED);
		e.WindowIsFloating = glfwGetWindowAttrib(window, GLFW_FLOATING);
		e.Time = glfwGetTime();
	}

	void GLGameEventInit()
	{
		m_Keys = new bool[GLGAME_INT_GLFW_KEY_COUNT];
		memset(m_Keys, 0, GLGAME_INT_GLFW_KEY_COUNT * sizeof(bool));
	}

	void GLGameKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
	{
		Event e;

		if (action == GLFW_PRESS)
		{
			m_Keys[key] = true;
			e.EventType = Event_KeyPressed;
		}

		else if (action == GLFW_RELEASE)
		{
			m_Keys[key] = false;
			e.EventType = Event_KeyReleased;
		}

		else
		{
			e.EventType = Event_Unknown;
		}

		e.KeyCode = key;
		GLGameSetDefaults(e, window);

		GameInternal::_IntAddEventToQueue(e);
	}

	void GLGameWindowSizeCallback(GLFWwindow* window, int width, int height)
	{
		Event e;

		e.EventType = Event_WindowResize;
		e.WindowSizeCoordsX = width;
		e.WindowSizeCoordsY = height;
		e.WindowIconified = glfwGetWindowAttrib(window, GLFW_ICONIFIED);
		e.WindowVisible = glfwGetWindowAttrib(window, GLFW_VISIBLE);
		e.WindowHasFocus = glfwGetWindowAttrib(window, GLFW_FOCUSED);
		glfwGetWindowPos(window, &e.WindowPositionX, &e.WindowPositionX);
		glfwGetFramebufferSize(window, &e.WindowFrameBufferX, &e.WindowFrameBufferY);
		glfwGetCursorPos(window, &e.MouseX, &e.MouseX);
		e.WindowIsMaximized = glfwGetWindowAttrib(window, GLFW_MAXIMIZED);
		e.WindowIsHovered = glfwGetWindowAttrib(window, GLFW_HOVERED);
		e.WindowIsFloating = glfwGetWindowAttrib(window, GLFW_FLOATING);

		GameInternal::_IntAddEventToQueue(e);
	}

	void GLGameMouseCallback(GLFWwindow* window, int button, int action, int mods)
	{
		Event e; 
		
		if (action == GLFW_PRESS)
		{
			e.EventType = Event_MouseButtonPressed;
		}

		else if (action == GLFW_RELEASE)
		{
			e.EventType = Event_MouseButtonReleased;
		}

		else
		{
			e.EventType = Event_Unknown;
		}

		e.MouseButton = button;
		GLGameSetDefaults(e, window);
		GameInternal::_IntAddEventToQueue(e);
	}

	void GLGameScrollCallback(GLFWwindow* window, double xoffset, double yoffset)
	{
		Event e; 

		e.EventType = Event_MouseScrollWheel;
		e.MouseScrollOffsetX = xoffset; 
		e.MouseScrollOffsetY = yoffset; 

		GLGameSetDefaults(e, window);
		GameInternal::_IntAddEventToQueue(e);
	}

	void GLGameDropCallback(GLFWwindow* window, int count, const char** paths)
	{
		Event e;
		_PathDrop pd;

		e.EventType = Event_PathDrop;
		pd.count = count;
		pd.paths = (char**) paths;
		e.PathDrops = pd;

		GLGameSetDefaults(e, window); 
		GameInternal::_IntAddEventToQueue(e);
	}

	void GLGameCursorPosCallback(GLFWwindow* window, double xpos, double ypos)
	{
		Event e;

		e.EventType = Event_CursorMoved;
		e.MouseX = xpos;
		e.MouseY = ypos;
		e.WindowIconified = glfwGetWindowAttrib(window, GLFW_ICONIFIED);
		e.WindowVisible = glfwGetWindowAttrib(window, GLFW_VISIBLE);
		e.WindowHasFocus = glfwGetWindowAttrib(window, GLFW_FOCUSED);
		glfwGetWindowPos(window, &e.WindowPositionX, &e.WindowPositionX);
		glfwGetWindowSize(window, &e.WindowSizeCoordsX, &e.WindowSizeCoordsY);
		glfwGetFramebufferSize(window, &e.WindowFrameBufferX, &e.WindowFrameBufferY);
		e.WindowIsMaximized = glfwGetWindowAttrib(window, GLFW_MAXIMIZED);
		e.WindowIsHovered = glfwGetWindowAttrib(window, GLFW_HOVERED);
		e.WindowIsFloating = glfwGetWindowAttrib(window, GLFW_FLOATING);
		e.Time = glfwGetTime();
		GameInternal::_IntAddEventToQueue(e);
	}

	void GLGameCursorEnterCallback(GLFWwindow* window, int entered)
	{
		Event e;

		e.EventType = Event_CursorEnter;
		e.CursorEntered = true;
		
		GLGameSetDefaults(e, window);
		GameInternal::_IntAddEventToQueue(e);
	}

	void GLGameWindowCloseCallback(GLFWwindow* window)
	{
		Event e;

		e.EventType = Event_WindowClose;
		e.WindowToClose = true;
		GLGameSetDefaults(e, window);
		GameInternal::_IntAddEventToQueue(e);
	}

	void GLGameFramebufferSizeCallback(GLFWwindow* window, int width, int height)
	{
		Event e;

		e.EventType = Event_FrameBufferResize;
		e.WindowFrameBufferX = width;
		e.WindowFrameBufferY = height;
		e.WindowIconified = glfwGetWindowAttrib(window, GLFW_ICONIFIED);
		e.WindowVisible = glfwGetWindowAttrib(window, GLFW_VISIBLE);
		e.WindowHasFocus = glfwGetWindowAttrib(window, GLFW_FOCUSED);
		glfwGetWindowPos(window, &e.WindowPositionX, &e.WindowPositionX);
		glfwGetWindowSize(window, &e.WindowSizeCoordsX, &e.WindowSizeCoordsY);
		glfwGetCursorPos(window, &e.MouseX, &e.MouseX);
		e.WindowIsMaximized = glfwGetWindowAttrib(window, GLFW_MAXIMIZED);
		e.WindowIsHovered = glfwGetWindowAttrib(window, GLFW_HOVERED);
		e.WindowIsFloating = glfwGetWindowAttrib(window, GLFW_FLOATING);
		e.Time = glfwGetTime();

		GameInternal::_IntAddEventToQueue(e);
	}

	void GLGameWindowPositionCallback(GLFWwindow* window, int xpos, int ypos)
	{
		Event e;

		e.EventType = Event_WindowPosition;
		e.WindowIconified = glfwGetWindowAttrib(window, GLFW_ICONIFIED);
		e.WindowVisible = glfwGetWindowAttrib(window, GLFW_VISIBLE);
		e.WindowHasFocus = glfwGetWindowAttrib(window, GLFW_FOCUSED);
		e.WindowPositionX = xpos;
		e.WindowPositionY = ypos;
		glfwGetWindowSize(window, &e.WindowSizeCoordsX, &e.WindowSizeCoordsY);
		glfwGetFramebufferSize(window, &e.WindowFrameBufferX, &e.WindowFrameBufferY);
		glfwGetCursorPos(window, &e.MouseX, &e.MouseX);
		e.WindowIsMaximized = glfwGetWindowAttrib(window, GLFW_MAXIMIZED);
		e.WindowIsHovered = glfwGetWindowAttrib(window, GLFW_HOVERED);
		e.WindowIsFloating = glfwGetWindowAttrib(window, GLFW_FLOATING);
		e.Time = glfwGetTime();

		GameInternal::_IntAddEventToQueue(e);
	}

	void GLGameWindowIconifiedCallback(GLFWwindow* window, int iconified)
	{
		Event e;

		e.EventType = Event_WindowIconified;
		e.WindowIconified = (bool) iconified;
		GLGameSetDefaults(e, window);
	}

	void GLGameWindowMaximizedCallback(GLFWwindow* window, int maximized)
	{
		Event e;

		e.EventType = Event_WindowMaximized;
		e.WindowIsMaximized = maximized;
		GLGameSetDefaults(e, window); 
	}
	
	void GLGameWindowFocusCallback(GLFWwindow* window, int focused)
	{
		Event e;

		e.EventType = Event_WindowInputFocus;
		e.WindowHasFocus = focused;
		GLGameSetDefaults(e, window);
	}
	
	bool* GLGameInternalGetKeyVal()
	{
		return m_Keys;
	}
}