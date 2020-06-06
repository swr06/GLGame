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
#include "Core\Engine\Logger\Log.h"
#include "Core\Engine\Rendering\BatchRendering.h"
#include "Core\Engine\Lighting\LightBatcher.h"
#include "Core\Engine\Lighting\Lighting.h"
#include "Core\Engine\Particles\Particles.h"

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

	struct CustomEvent
	{
		int m_Event;
		long long m_Frame;
	};

	class Game
	{
	public:

		Game(const Game&) = delete; // To prevent multiple initializations
		Game() : m_CurrentScene(nullptr), m_DisplayFPS(true), m_GameWindow(nullptr), m_KeyHoldEventBuffer(nullptr) {}
		~Game();

		void Render(bool should_clear = true);
		void StartSceneEditor();

		void SetVSync(bool vsync, bool log = true);
		void DisplayFpsOnWindowTitleBar(bool display);
		void SetCurrentScene(Scene& scene);
		bool GameWindowShouldClose();

		void SetBlend(bool blend);
		void SetBlendFunction(GLenum blendfuncS, GLenum blendfuncD);

		bool IsThereCollision(Object& obj_1, Object& obj_2);

		// Event handling

		// It is recommended to handle the key hold/repeat events in the OnEvent callback.
		bool KeyIsBeingPressed(int Key);
		
		Scene& GetCurrentScene();
		Scene* GetCurrentScene() const { return m_CurrentScene; };
		Camera* GetCurrentCamera();
		GLFWwindow* GetGameWindow() const { return m_GameWindow; }

		vector<string>* GetGlobalObjectIDs() { return &m_ObjectItemNames; }
		vector<string>* GetGlobalSpriteIDs() { return &m_SpriteItemNames; }

		bool ShouldDrawImGui() const { return m_DisplayImGui; }

		// Helper Functions : 





		// Internal functions.

		// Internal function. Not meant to use.
		void _RegisterObject(Object* object);

		// Internal function. Not meant to use.
		void _RegisterSprite(Sprite* sprite);

		// Internal function. Not meant to use.
		void _RegisterScene(Scene* scene);

		// Internal function. Not meant to use.
		void _QueueEvent(Event e);

		// Internal function. Not meant to use.
		GameInternal::_GlobalGameData _GetGlobalData();

		// Internal function. Not meant to use.
		// Gets a sprite from the global array based on an id
		Sprite* _GetSpriteFromArr(const string& id);

		// Internal function. Not meant to use.
		// Gets an object from the global array based on an id
		Object* _GetObjectFromArr(const string& id);

	private:
		void PollEvents();
		void ConvertCoordinates(glm::vec3 coordinates, glm::vec3 range);

		bool m_ImGuiInitialized = false;
		bool m_GameAlreadyDestroyed = false;
		vector<Event> m_EventQueue;
		GLFWwindow* m_SceneEditorWindow;

		unordered_map <uint32_t, Background*> m_GlobalBackgrounds;
		unordered_map <uint32_t, Scene*> m_GlobalScenes; // Internal vector
		unordered_map <string, Object*> m_GlobalObjects; // Internal vector
		unordered_map <string, Sprite*> m_GlobalSprites; // Internal vector
		vector<string> m_ObjectItemNames;
		vector<string> m_SpriteItemNames;

		long long m_FpsCount = 0;
		bool m_OpenGLInitialized = false;
		Shader m_DefaultObjectShader;
		Shader m_DefaultBackgroundShader;

		bool* m_KeyHoldEventBuffer = nullptr;
		
	protected : 
		
		void Init(int w, int h, bool can_resize = true, string title = "Test Game", bool start_SE = false, bool use_imgui = false, ImGuiStyle imgui_style = ImGuiStyleDark);

		int m_GameWindowWidth;
		int m_GameWindowHeight;
		string m_WindowTitle;

		bool m_DisplayFPS = true;
		bool m_DisplayImGui = false;
		bool m_Vsync = false;
		bool m_CanResize = true;

		ImGuiStyle m_IMStyle;
		ImGuiContext* m_IMContext;
		Scene* m_CurrentScene;
		GLFWwindow* m_GameWindow;
		SpriteBatcher* m_SpriteBatcher;
		SpriteBatcher* m_CustomShaderBatcher;
		LightBatcher* m_LightBatcher;

		GameDebugInfo m_DebugInfo;

		// Virtual Functions for event handling
		virtual void OnInitialize(double ts) {}
		virtual void OnGameStart(double ts) {}
		virtual void OnCustomEvent(long long frame) {} // TODO
		virtual void SceneLoader() {}
		virtual void OnObjectMove(Object* object) {} // TODO
		virtual void OnEvent(Event e) {}
		virtual void OnImGuiRender(long long frame) {}
		virtual void OnFrameAdvance(long long frame) {}
		virtual void OnGameDestroy(double ts) {}
	};

	typedef Object Entity;
}