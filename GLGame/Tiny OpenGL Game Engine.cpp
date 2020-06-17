#include "Core\Engine\Game\Game.h"

using namespace GLGame;
using namespace std;

// The game camera
Camera cam(0.0f, 800.0f, 0.0f, 600.0f);

class DemoGame : public Game
{
public:
	DemoGame()
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
	}
};

// Create the light demo game
DemoGame LightDemo;

int main()
{
	TextureAtlas player_idle("Demo\\Assets\\player_idle.png", 32,32);
	TextureAtlas player_run("Demo\\Assets\\player_run.png", 32, 32);

	Animation player_idle_animation({
		player_idle.Sample(glm::vec2(0.1,0), glm::vec2(1,1)),
		player_idle.Sample(glm::vec2(1.1,0), glm::vec2(2,1)),
		player_idle.Sample(glm::vec2(2.1,0), glm::vec2(3,1)),
		player_idle.Sample(glm::vec2(3.1,0), glm::vec2(4,1)),
		});

	Sprite player_idle_spr("Player Idle", player_idle_animation, 10);
	Object player("Player");

	player.SetSprite(player_idle_spr);

	Scene scene;

	scene.AddObjectAtPosition(player, 0, glm::vec3(200, 200, 0));

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