#pragma once

#define GLEW_STATIC

#include <unordered_map>
#include <map>
#include <vector>
#include <string>
#include <thread>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

// GLEW
#include <GL/glew.h> 

#include <GLFW/glfw3.h>

#include "Core\OpenGL Classes\GLDebug\GLDebug.h"

#include "Core\Dependencies\imgui\imgui.h"
#include "Core\Dependencies\imgui\imgui_impl_glfw.h"
#include "Core\Dependencies\imgui\imgui_impl_opengl3.h"

#include "Core\Engine\Logger\Log.h"
#include "Core\Engine\Macros\Macros.h"
#include "Core\Engine\Collision\AABB.h"
#include "Core\OpenGL Classes\Texture\Texture.h"
#include "Core\Engine\Rendering\BatchRendering.h"
#include "Core\Engine\Rendering\Rendering.h"
#include "Core\Engine\Object\Object.h"
#include "Core\Engine\Sprite\Sprite.h"
#include "Core\Engine\Scene\SceneData.h"
#include "Core\Engine\Scene\SceneFileParser.h"

namespace GLGame
{
	namespace SceneEditor
	{
		using namespace std;

		enum SceneEditorItemTypes
		{
			SE_ObjectType,
			SE_SpriteType,
		};

		struct SceneEditorRenderItem
		{
			Object* obj = nullptr; 
			Sprite* spr = nullptr;
			float x;
			float y;
			int layer;
			SceneEditorItemTypes item_type;
		};

		GLFWwindow* _Init(GLFWwindow* share_window, ImGuiContext* context);
		GLFWwindow* InitSceneEditor(unordered_map<string, Object*>* global_objects, unordered_map<string, Sprite*>* global_sprites, vector<string>* objid_list, vector<string>* sprid_list, GLFWwindow* window, ImGuiContext* context);
		void _SetSEImGuiFlags();
		void _DrawSEWidgets();
		void _DrawSEMenuBar();
		bool RenderSceneEditor();
		bool SceneEditorAlive();
		void RenderSceneEditorItems();
		void _ShowModalWindows();

		// GLFW Callbacks
		void SEWindowResizeCallback(GLFWwindow* window, int width, int height);
		void SEKeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods);
		void SEMouseCallback(GLFWwindow* window, int button, int action, int mods);
		void SEScrollCallback(GLFWwindow* window, double xoffset, double yoffset);
		void SEWindowCloseCallback(GLFWwindow* window);
		void SECursorPosCallback(GLFWwindow* window, double xpos, double ypos);
	}
}