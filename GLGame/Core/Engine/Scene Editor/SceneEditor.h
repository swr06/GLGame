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
	struct GameDebugInfo
	{
		long VerticesCount = 0;
		long IndicesCount = 0;
		unsigned int QuadCount = 0;
		long long CurrentFrame = 0;
		double CurrentTS = 0;
		unsigned int ObjectsDrawn = 0;
		unsigned int SpritesDrawn = 0;
		unsigned int ParticlesDrawn = 0;
		unsigned int LightsDrawn = 0;
		double RenderTime;
	};

	namespace SceneEditor
	{
		using namespace std;
		
		GLFWwindow* InitSceneEditor(GameDebugInfo* debug_info, unordered_map<string, Object*>* global_objects, unordered_map<string, Sprite*>* global_sprites, unordered_map<string, Background*>* global_bgs, vector<string>* objid_list, vector<string>* sprid_list, vector<string>* bgid_list, GLFWwindow* window, ImGuiContext* context);
		void _SetSceneEditorCloseFlag(bool val);
		bool RenderSceneEditor();
		bool SceneEditorAlive();
	}
}