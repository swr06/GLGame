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
		void _IntDeregisterScene(Scene* scene);
	}

	namespace SceneParser
	{
		void ParseSceneData(const string& scene_file_pth, Scene* scene);
	}

	struct _SceneData
	{
		// Item Data
		map<int, unordered_map<string, vector<SceneDataItem>>>* scene_items;
		map<int, SceneBackground>* scene_backgrounds;

		// Light data
		vector<Light*> *scene_lights;
		vector<BlinkingLight*> *scene_blinking_lights;
		vector<PulsatingLight*> *scene_pulsating_lights;
		vector<PulsatingLightRadius*> *scene_pulsating_radius_lights;
	};

	class Scene
	{
	public : 

		Scene(); 
		~Scene();

		// To handle scene files
		void LoadSceneFile(string scene_path);

		// Scene set functions
		void SetSceneCamera(Camera &camera); 
		void SetSceneCamera(Camera *camera); 
		void SetSceneAmbientLight(const glm::vec4& ambient_light);
		void AddSceneBackground(Background& background, int depth);
		void AddSceneBackground(Background* background, int depth);

		// Object Functions
		void AddObjectAtPosition(Object &object, int layer, const glm::vec3& position);
		void AddSpriteAtPosition(Sprite& sprite, int layer, const glm::vec3& position);
		void AddLightAtPosition(Light& light);
		void AddBlinkingLightAtPosition(BlinkingLight& blinking_light);
		void UpdateObjectPosition(Object& object, const glm::vec3& position);
		void IncrementObjectPosition(Object& object, const glm::vec3& increment);

		// Camera getter functions
		inline Camera* GetSceneCamera() { return m_SceneCamera; }; 
		inline uint32_t GetSceneID() const { return m_ID; }
		inline const glm::vec4 GetSceneAmbientLight() const { return m_AmbientLight; }

		// To get all the scene data
		// This function is only meant to be used internally
		inline _SceneData IntGetSceneData() { return m_SceneData; }

	private : 

		glm::vec4 m_AmbientLight;
		map<int, SceneBackground> m_SceneBackgrounds;
		map<int, unordered_map<string, vector<SceneDataItem>>> m_SceneItems;

		// Scene lighting
		vector<Light*> m_SceneLights;
		vector<BlinkingLight*> m_SceneBlinkingLights;
		vector<PulsatingLight*> m_ScenePulsatingLights;
		vector<PulsatingLightRadius*> m_ScenePulsatingRadiusLights;

		_SceneData m_SceneData;
		
		Camera* m_SceneCamera;
		uint32_t m_ID; 
	};
}