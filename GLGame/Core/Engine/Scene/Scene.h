#include <vector>
#include <unordered_map>
#include <map>
#include <string>
#include <iostream>
#include <fstream>
#include <array>

#include "Core\OpenGL Classes\GLDebug\GLDebug.h"
#include "Core\Engine\Scene\SceneData.h"
#include "Core\Engine\Sprite\Sprite.h"
#include "Core\Engine\Object\Object.h"
#include "Core\Engine\Instance\Instance.h"
#include "Core\Engine\Camera\Camera.h"
#include "Core\Engine\Background\Background.h"
#include "Core\Engine\Lighting\Lighting.h"

namespace GLGame
{
	static const int max_layers = 20;
	static uint32_t GenerateSceneID(); 

	class Scene;

	namespace GameInternal
	{
		void _IntRegisterScene(Scene* scene);
	}

	class Scene
	{
	public : 

		Scene(); 
		void FromSceneFile(string scene_path);

		// Scene set functions
		void SetSceneCamera(Camera &camera); 
		void SetSceneCamera(Camera *camera); 
		void AddSceneBackground(Background& background, int depth);
		void AddSceneBackground(Background* background, int depth);

		// Object Functions
		void AddObjectAtPosition(Object &object, int layer, const glm::vec3& position);
		void AddLightAtPosition(Light& light);
		void AddBlinkingLightAtPosition(BlinkingLight& blinking_light);
		void UpdateObjectPosition(Object& object, const glm::vec3& position);
		void IncrementObjectPosition(Object& object, const glm::vec3& increment);

		// Camera getter functions
		inline Camera* GetSceneCamera() { return m_SceneCamera; }; 
		inline uint32_t GetSceneID() const { return m_ID; }

		// Internal functions
		map<int, unordered_map<string, vector<SceneDataItem>>>* IntGetSceneData() { return &m_SceneItems; };
		map<int, SceneBackground>& IntGetSceneBackgroundData() { return m_SceneBackgrounds; }
		vector<Light*>& IntGetSceneLightData() { return m_SceneLights; }
		vector<BlinkingLight*>& IntGetSceneBlinkingLightData() { return m_SceneBlinkingLights; }

	private : 

		map<int, SceneBackground> m_SceneBackgrounds;
		map<int, unordered_map<string, vector<SceneDataItem>>> m_SceneItems;
		vector<Light*> m_SceneLights;
		vector<BlinkingLight*> m_SceneBlinkingLights;

		Camera* m_SceneCamera;
		uint32_t m_ID; 
	};
}