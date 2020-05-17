#pragma once

#define GLEW_STATIC

#include <iostream>
#include <string>
#include <unordered_map>
#include <vector>
#include <thread>
#include <GL/glew.h>

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Core\OpenGL Classes\GLDebug\GLDebug.h"

// Imgui is already included in the scene editor files

// Also included in the scene editor files
// #include "Core\Engine\Scene\Scene.h"
//#include "Core\Engine\Rendering\Rendering.h"

#include "Core\Engine\Scene Editor\SceneEditor.h"
#include "Core\OpenGL Classes\FPS\Fps.h"
#include "Core\Engine\Events\Events.h"
#include "Core\Engine\Collision\AABB.h"
#include "Core\Engine\Macros\Macros.h"
#include "Core\Engine\Object\Object.h"
#include "Core\Engine\Sprite\Sprite.h"
#include "Core\Engine\Background\Background.h"
#include "Core\Engine\Animation\Animation.h"
#include "Core\Engine\TextureAtlas\TextureAtlas.h"
#include "Core\Engine\Log\Log.h"
#include "Core\Engine\Rendering\BatchRendering.h"

namespace GLGame
{
	enum ImGuiStyle
	{
		ImGuiStyleDark,
		ImGuiStyleLight,
		ImGuiStyleClassic,
	};

	namespace GameInternal
	{
		struct _GlobalGameData
		{
			unordered_map <uint32_t, Background*>* gBackgrounds;
			unordered_map <uint32_t, Scene*>* gScenes;
			unordered_map <string, Object*>* gObjects;
			unordered_map <string, Sprite*>* gSprites;
		};
	}

	class Game
	{
	public:

		Game(const Game&) = delete; // To prevent multiple initializations
		Game(int width, int height, const string title = string("GLGame V01"), bool can_resize = true, bool use_imgui = false, ImGuiStyle imgui_style = ImGuiStyleDark) : m_CurrentScene(nullptr), m_DisplayFPS(true), m_GameWindow(nullptr), m_KeyHoldEventBuffer(nullptr)
		{
			Init(width, height, can_resize, title, use_imgui, imgui_style);
		}

		~Game();

		void Render(bool should_clear = true);
		void Init(int w, int h, bool can_resize, string title, bool use_imgui, ImGuiStyle imgui_style);
		void StartSceneEditor();

		void ConvertCoordinates(glm::vec3 coordinates, glm::vec3 range);
		void SetVSync(bool vsync, bool log = true);
		void DisplayFpsOnWindowTitleBar(bool display);
		void SetCurrentScene(Scene& scene);
		bool GameWindowShouldClose();

		void SetBlend(bool blend);
		void SetBlendFunction(GLenum blendfuncS, GLenum blendfuncD);

		bool IsThereCollision(Object& obj_1, Object& obj_2);

		// Event handling
		bool KeyIsBeingPressed(int Key);
		void PollEvents();

		// Virtual Functions
		void OnEvent(Event e);
		void OnImGuiRender(long long frame);
		void OnFrameAdvance(long long frame);

		Scene& GetCurrentScene();
		Scene* GetCurrentScene() const { return m_CurrentScene; };
		Camera* GetCurrentCamera();
		GLFWwindow* GetGameWindow() const { return m_GameWindow; }

		vector<string>* GetGlobalObjectIDs() { return &m_ObjectItemNames; }
		vector<string>* GetGlobalSpriteIDs() { return &m_SpriteItemNames; }
		void _RegisterObject(Object* object);
		void _RegisterSprite(Sprite* sprite);
		void _RegisterScene(Scene* scene);
		void _QueueEvent(Event e);
		GameInternal::_GlobalGameData _GetGlobalData();

		bool ShouldDrawImGui() const { return m_DisplayImGui; }
		Object* _GetObjectFromArr(const string& id);

	private:
		uint32_t m_GameWindowWidth = 0;
		uint32_t m_GameWindowHeight = 0;
		bool m_DisplayFPS;
		bool m_DisplayImGui = false;
		bool m_ImGuiInitialized = false;
		bool m_Vsync = false;
		vector<Event> m_EventQueue;
		ImGuiContext* m_IMContext;

		unordered_map <uint32_t, Background*> m_GlobalBackgrounds;
		unordered_map <uint32_t, Scene*> m_GlobalScenes; // Internal vector
		unordered_map <string, Object*> m_GlobalObjects; // Internal vector
		unordered_map <string, Sprite*> m_GlobalSprites; // Internal vector
		vector<string> m_ObjectItemNames;
		vector<string> m_SpriteItemNames;

		Scene* m_CurrentScene;
		GLFWwindow* m_GameWindow;

		long long m_FpsCount = 0;
		bool m_OpenGLInitialized = false;
		Shader m_DefaultObjectShader;
		Shader m_DefaultBackgroundShader;

		bool* m_KeyHoldEventBuffer = nullptr;
		SpriteBatcher *m_Batcher;
	};
}