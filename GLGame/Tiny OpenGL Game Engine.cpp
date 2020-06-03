#include "Core\Engine\Game\Game.h"
#include <GL/glew.h>

using namespace GLGame;
using namespace std;

const int game_width = 800;
const int game_height = 600;

void EventCallback(Event e);

class game_ : public Game
{
public : 
	game_() 
	{
		Init(800, 600, true, "Test Game", false, ImGuiStyleDark);
	}

	void OnGameDestroy(double ts) override
	{
		cout << "Game Destroyed! TS : " << ts;
	}

	void OnFrameAdvance(long long frame) override
	{
		/*static ParticleProps particle;

		particle.ColorBegin = { 1 / 255.0f, 1 / 255.0f, 1 / 255.0f, 1.0f };
		particle.ColorEnd = { 254 / 255.0f, 109 / 255.0f, 41 / 255.0f, 1.0f };
		particle.SizeBegin = 10.0f, particle.SizeVariation = 0.5f, particle.SizeEnd = 50.0f;
		particle.LifeTime = 1.0f;
		particle.Velocity = { 1.0f, 1.0f };
		particle.VelocityVariation = { 3.0f, 1.0f };
		particle.Position = { 100.0f, 100.0f };

		static ParticleSystem PS;

		for (int i = 0; i < 10; i++)
			PS.Emit(particle);

		static Camera cam(0.0f, 800.0f, 0.0f, 600.0f);

		PS.OnRender(cam);
		PS.OnUpdate(glfwGetTime());*/

	}

	void OnEvent(Event e) override
	{
		EventCallback(e);
	}

	void OnGameStart(double ts) override
	{
		
	}
	
private : 

};

game_ game;

Camera cam(0.0f, game_width, 0.0f, game_height);
Object obj("Object_1", true);
Object obj_2("Object_2", true);
Object obj_3("Object_3", true);
Background* bg;

bool increment_pos = true;
Event_Type type = Event_Key;

void EventCallback(Event e)
{
	const float move_factor = 4;
	const int move_factor_camera = 4;
	glm::vec3 pos = game.GetCurrentCamera()->GetPosition();

	if (e.EventType == Event_WindowResize || e.EventType == Event_FrameBufferResize)
	{
		cam.SetProjection(0, e.WindowFrameBufferX, 0, e.WindowFrameBufferY);
		glViewport(0, 0, e.WindowFrameBufferX, e.WindowFrameBufferY);
	}

	if (increment_pos)
	{
		if (e.EventType == type && e.KeyCode == GLFW_KEY_LEFT)
		{
			obj_2.IncrementPosition(glm::vec3(-move_factor, 0.0f, 0.0f));
		}

		else if (e.EventType == type && e.KeyCode == GLFW_KEY_RIGHT)
		{
			obj_2.IncrementPosition(glm::vec3(move_factor, 0.0f, 0.0f));
		}

		else if (e.EventType == type && e.KeyCode == GLFW_KEY_UP)
		{
			obj_2.IncrementPosition(glm::vec3(0.0f, move_factor, 0.0f));
		}

		else if (e.EventType == type && e.KeyCode == GLFW_KEY_DOWN)
		{
			obj_2.IncrementPosition(glm::vec3(0.0f, -move_factor, 0.0f));
		}


		else if (e.EventType == type && e.KeyCode == GLFW_KEY_A)
		{
			cam.SetPosition(glm::vec3(pos.x + move_factor_camera, pos.y, pos.z));
		}

		else if (e.EventType == type && e.KeyCode == GLFW_KEY_D)
		{
			cam.SetPosition(glm::vec3(pos.x - move_factor_camera, pos.y, pos.z));
		}

		else if (e.EventType == type && e.KeyCode == GLFW_KEY_W)
		{
			cam.SetPosition(glm::vec3(pos.x, pos.y - move_factor_camera, pos.z));
		}

		else if (e.EventType == type && e.KeyCode == GLFW_KEY_S)
		{
			cam.SetPosition(glm::vec3(pos.x, pos.y + move_factor_camera, pos.z));
		}

		else if (e.EventType == type && e.KeyCode == GLFW_KEY_L)
		{
			bg->IncrementPosition(glm::vec3(5.0f, 0.0f, 0.0f));
		}
	}
}

int main()
{
	//Light light_1(glm::vec3(mx, h - my, 0.0f), glm::vec4(1.0f, 0.0f, 0.0f, 1.0f), 200.0f);
	Light light_2(glm::vec3(400.0f, 400.0f, 0.0f), glm::vec4(0.0f, 1.0f, 0.0f, 2.0f), 400.0f);
	Light light_3(glm::vec3(600.0f, 400.0f, 0.0f), glm::vec4(1.0f, 1.0f, 1.0f, 1.0f), 800.0f);

	Light light_t(glm::vec3(100.0f, 300.0f, 0.0f), glm::vec4(0.0f, 1.0f, 0.0f, 1.0f), 100.0f);
	BlinkingLight pLight(light_t, 1, 30, 1.0f);
	Texture tex1, tex2, tile_sheet;
	Scene scene; 
	//Animation ani({&tex1, &tex2});

	Animation ani({ 
		(string)"Core\\Resources\\Animations\\Yoda\\Frame (1).gif", 
		(string)"Core\\Resources\\Animations\\Yoda\\Frame (2).gif", 
		(string)"Core\\Resources\\Animations\\Yoda\\Frame (3).gif", 
		(string)"Core\\Resources\\Animations\\Yoda\\Frame (4).gif", 
		(string)"Core\\Resources\\Animations\\Yoda\\Frame (5).gif", 
		(string)"Core\\Resources\\Animations\\Yoda\\Frame (6).gif", 
		(string)"Core\\Resources\\Animations\\Yoda\\Frame (7).gif", 
		(string)"Core\\Resources\\Animations\\Yoda\\Frame (8).gif", 
		(string)"Core\\Resources\\Animations\\Yoda\\Frame (9).gif", 
		(string)"Core\\Resources\\Animations\\Yoda\\Frame (10).gif", 
		(string)"Core\\Resources\\Animations\\Yoda\\Frame (11).gif", 
		(string)"Core\\Resources\\Animations\\Yoda\\Frame (12).gif", 
		(string)"Core\\Resources\\Animations\\Yoda\\Frame (13).gif", 
		(string)"Core\\Resources\\Animations\\Yoda\\Frame (14).gif", 
		});

	//bg = new Background("Core\\Resources\\grass_block.png");

	scene.SetSceneCamera(cam);
	scene.SetSceneAmbientLight(glm::vec4(0.5f, 0.5f, 0.5f, 1.0f));
// 	scene.AddLightAtPosition(light_2);
// 	scene.AddLightAtPosition(light_3);
// 	scene.AddBlinkingLightAtPosition(pLight);
// 	scene.AddSceneBackground(bg, 1);

	tex1.CreateTexture("Core\\Resources\\tree.png");
	tex2.CreateTexture("Core\\Resources\\ghost.png"); // Alpha = 50.0%

	Sprite spr("spr_1", ani, 5); 
	Sprite spr_2("spr_2", tex2);
	Sprite spr_3("spr_3", ani, 10);
	                               
	obj.SetSprite(spr); // tex1
	obj_2.SetSprite(spr_2); // tex2
	obj_3.SetSprite(spr_3);

	scene.LoadSceneFile("TestScene.sce");

	game.SetCurrentScene(scene);
	game.DisplayFpsOnWindowTitleBar(true);
	game.SetBlend(true);
	game.SetVSync(true);

	while (1)
	{
		game.Render(); 

		if (game.IsThereCollision(obj, obj_2))
		{
			cout << "\nCOLLISION!\n";
		}
	}
} 

