/*
Written by : Samuel Rasquinha
Age : 14
Description : A platformer demo for my Game engine (GLGame) 
*/

#include "Core\Engine\Game\Game.h"

using namespace GLGame;
using namespace std;

const int gravity = 5.0f;
const int jump = 10.0f;

// The game camera
Camera cam(0.0f, 800.0f, 0.0f, 600.0f);

void PlayerUpdate(Event e);

class DemoGame : public Game
{
public:
	DemoGame()
	{
		Init(800, 600, true, "GLGame Platformer Demo V01", false);
	}

	void OnEvent(Event e) override
	{
		// When the window is resized, set the viewport and adjust the projection matrix of the camera
		if (e.EventType == Event_WindowResize || e.EventType == Event_FrameBufferResize)
		{
			glViewport(0, 0, e.WindowFrameBufferX, e.WindowFrameBufferY);
			cam.SetProjection(0.0f, (float)this->GetWindowWidth(), 0.0f, (float)this->GetWindowHeight());
		}

		// Update the player based on when an event occurs
		PlayerUpdate(e);
	}
};

// Create the platformer demo game
DemoGame PlatformerDemo;

// To sample from the texture atlases
TextureAtlas player_idle_atlas("Demo\\Assets\\player_idle.png", 29, 35);
TextureAtlas player_run_atlas("Demo\\Assets\\player_run.png", 40, 35);
TextureAtlas player_run_atlas_flipped("Demo\\Assets\\player_run_flipped.png", 40, 35);

// The player's idle animation
Animation player_idle_animation({
	player_idle_atlas.Sample(glm::vec2(0,0), glm::vec2(1.0,1)),
	player_idle_atlas.Sample(glm::vec2(1,0), glm::vec2(2.0,1)),
	player_idle_atlas.Sample(glm::vec2(2,0), glm::vec2(3.0,1)),
	player_idle_atlas.Sample(glm::vec2(3,0), glm::vec2(4.0,1)),
	});

// The player's running animations
Animation player_run_animation({
	player_run_atlas.SampleCustom(glm::vec2(0,0), glm::vec2(34,35)),
	player_run_atlas.SampleCustom(glm::vec2(37,0), glm::vec2(75,36)),
	player_run_atlas.SampleCustom(glm::vec2(79,0), glm::vec2(117,35)),
	player_run_atlas.SampleCustom(glm::vec2(119,0), glm::vec2(159,35)),
	player_run_atlas.SampleCustom(glm::vec2(161,0), glm::vec2(201,35)),
	player_run_atlas.SampleCustom(glm::vec2(206,1), glm::vec2(244,35)),
	});

Animation player_run_animation_flipped({
	player_run_atlas_flipped.SampleCustom(glm::vec2(206,1), glm::vec2(244,35)),
	player_run_atlas_flipped.SampleCustom(glm::vec2(161,0), glm::vec2(201,35)),
	player_run_atlas_flipped.SampleCustom(glm::vec2(119,0), glm::vec2(159,35)),
	player_run_atlas_flipped.SampleCustom(glm::vec2(79,0), glm::vec2(117,35)),
	player_run_atlas_flipped.SampleCustom(glm::vec2(37,0), glm::vec2(75,36)),
	player_run_atlas_flipped.SampleCustom(glm::vec2(0,0), glm::vec2(34,35)),
});

// The block
Texture block("Demo\\Assets\\block.png");

Sprite player_idle_spr("Player Idle", player_idle_animation, 5);
Sprite player_run_spr("Player Run", player_run_animation, 5);
Sprite player_run_spr_f("Player Run Flipped", player_run_animation_flipped, 5);
Sprite wall_block_spr("Wall block", &block);

// Inherits from the GLGame::Object class
class Player : public Object
{
public:

	Player() : Object("Player") {}

	// Direction of the player. 0 = LEFT, 1 = RIGHT
	int dir;
	int score;
	int lives;
};

// Objects and Sprites
Player player;
Object wall_block("Wall Block", wall_block_spr);
Object wall_block_2("Wall Block 2", wall_block_spr);
Background main_background("Demo\\Assets\\background.png", "main_background");

bool player_is_jumping = false;
bool player_started_jumping = false;
int player_jump_increment_count = 0;

// Updates some player flags based on an event
void PlayerUpdate(Event e)
{
	if (e.EventType == Event_KeyPressed && e.KeyCode == GLFW_KEY_SPACE)
	{
		if (!player_started_jumping && !player_is_jumping)
			player_is_jumping = true;
	}

	else if (e.EventType == Event_KeyReleased && e.KeyCode == GLFW_KEY_SPACE)
	{
		player_is_jumping = false;
		player_started_jumping = false;
	}

	else if (e.EventType == Event_Key && e.KeyCode == GLFW_KEY_LEFT )
	{
		if (!player_is_jumping)
		{
			player.dir = 0;
			player.SetSprite(player_run_spr_f);
			player.IncrementPosition(glm::vec3(-4.0f, 0.0f, 0.0f));
		}
	}

	else if (e.EventType == Event_Key && e.KeyCode == GLFW_KEY_RIGHT)
	{
		if (!player_is_jumping)
		{
			player.dir = 1;
			player.SetSprite(player_run_spr);
			player.IncrementPosition(glm::vec3(4.0f, 0.0f, 0.0f));
		}
	}
}


int main()
{
	// Set the default idle sprite
	player.SetSprite(player_idle_spr);

	Scene scene;

	// Load the scene file
	scene.LoadSceneFile("Demo\\Assets\\DemoScene.sce");

	// Setting some flags
	PlatformerDemo.SetCurrentScene(scene);
	PlatformerDemo.DisplayFpsOnWindowTitleBar(true);
	PlatformerDemo.SetBlend(true);
	PlatformerDemo.SetVSync(true);

	// breaks when the game window should close
	while (!PlatformerDemo.GameWindowShouldClose())
	{
		// This needs to be called every frame
		PlatformerDemo.Render();

		// If there is a collision with the side block, increment the position accordingly
		if (PlatformerDemo.IsThereCollision(player, wall_block_2))
		{
			if (player.dir == 0)
			{
				player.IncrementPosition(glm::vec3(4.0f, 0.0f, 0.0f));
			}

			else if (player.dir == 1)
			{
				player.IncrementPosition(glm::vec3(-4.0f, 0.0f, 0.0f));
			}
		}

		// If the player has to jump
		if (player_is_jumping)
		{
			// Animate his jump for 10 frames
			if (player_started_jumping)
			{
				if (player_jump_increment_count > 10)
				{
					player_started_jumping = false;
					player_is_jumping = false;
					player_jump_increment_count = 0;
				}

				// Flip the player sprite
				if (player.dir == 0) // the direction was left
					player.IncrementPosition(glm::vec3(-3.0f, 5, 0.0f));
				else if (player.dir == 1)
					player.IncrementPosition(glm::vec3(3.0f, 5, 0.0f));
				else 
					player.IncrementPosition(glm::vec3(0.0f, 5, 0.0f));

				// To keep track of the loop count. The jump has to be animated.
				player_jump_increment_count++;
			}

			// If the jump is legal, then start the jump animation
			if (PlatformerDemo.IsThereCollision(player, wall_block))
			{
				player.IncrementPosition(glm::vec3(0.0f, 3, 0.0f));
				player_started_jumping = true;
			}
		}

		else
		{
			// Add a small delay to the sprite shifting
			if (PlatformerDemo.GetCurrentFrame() % 15 == 0)
			{
				player.SetSprite(player_idle_spr);
			}

			// Set the player's direction to an invalid value
			player.dir = -1;

			// To simulate gravity
			if (!PlatformerDemo.IsThereCollision(player, wall_block))
			{
				player.IncrementPosition(glm::vec3(0.0f, -gravity, 0.0f));
			}
		}
	}

	// Program returns
	return 0;
}
