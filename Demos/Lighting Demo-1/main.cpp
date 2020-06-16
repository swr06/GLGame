#include "Core\Engine\Game\Game.h"

using namespace GLGame;
using namespace std;

// The game camera
Camera cam(0.0f, 800.0f, 0.0f, 600.0f);

// The light that moves with the mouse pointer
Light moving_light(glm::vec3(100.0f, 300.0f, 0.0f), glm::vec4(1.0f, 0.0f, 0.0f, 4.0f), 400.0f);

class LightDemoGame : public Game
{
public : 
	LightDemoGame()
	{
		Init(800, 600, true, "GLGame Lighting Demo V01", false);
	}

	void OnEvent(Event e) override
	{
		// When the window is resized, set the viewport and adjust the projection matrix of the camera
		if (e.EventType == Event_WindowResize || e.EventType == Event_FrameBufferResize)
		{
			glViewport(0, 0, e.WindowFrameBufferX, e.WindowFrameBufferY);
			cam.SetProjection(0, e.WindowFrameBufferX, 0, e.WindowFrameBufferY);
		}

		if (e.EventType == Event_CursorMoved)
		{
			// The mouse Y has to be subtracted from the size of the window because the "top" of the window is 0 in glfw but is the opposite in the camera.
			moving_light.m_Position = glm::vec3(e.MouseX, e.WindowFrameBufferY - e.MouseY, 0.0f);
		}
	}
};

// Create the light demo game
LightDemoGame LightDemo;

int main()
{
	// Creating the lights 
	// Arguments : 
	// [1] glm::vec3 : Position
	// [2] glm::vec4 : Color (The higher the last variable glm::vec4.w) the higher the intensity will be
	// [3] float : The diameter of the light

	Light light_1(glm::vec3(300.0f, 400.0f, 0.0f), glm::vec4(0.0f, 0.0f, 1.0f, 4.0f), 400.0f);
	Light light_2(glm::vec3(500.0f, 400.0f, 0.0f), glm::vec4(0.0f, 1.0f, 0.0f, 4.0f), 400.0f);
	Light light_3(glm::vec3(700.0f, 400.0f, 0.0f), glm::vec4(1.0f, 1.0f, 1.0f, 4.0f), 400.0f);

	// Creating the scene
	Scene scene;

	// Add the lights to the scene
	scene.AddLightAtPosition(light_1);
	scene.AddLightAtPosition(light_2);
	scene.AddLightAtPosition(light_3);
	scene.AddLightAtPosition(moving_light);

	// Setting the scene and setting some flags
	LightDemo.SetCurrentScene(scene);
	LightDemo.DisplayFpsOnWindowTitleBar(true);
	LightDemo.SetBlend(true);
	LightDemo.SetVSync(true);

	// breaks when the game window should close
	while (!LightDemo.GameWindowShouldClose())
	{
		// This needs to be called every frame
		LightDemo.Render();
	}

	return 0;
} 