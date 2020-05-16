#include "Scene.h"

namespace GLGame
{
	static const int window_width = 800;
	static const int window_height = 600;

	Scene::Scene()
	{
		m_SceneCamera = new Camera(0, window_width, 0, window_height);
		m_ID = GenerateSceneID();
		GameInternal::_IntRegisterScene(this);
	}

	void Scene::FromSceneFile(string scene_path)
	{
		// TODO : Parse .scene file
	}

	void Scene::SetSceneCamera(Camera& camera)
	{
		m_SceneCamera = &camera;
	}

	void Scene::SetSceneCamera(Camera* camera)
	{
		m_SceneCamera = camera;
	}

	void Scene::AddSceneBackground(Background& background, int depth)
	{
		SceneBackground bg;

		bg.background = &background;
		bg.position = glm::vec3(0, 0, 0);

		m_SceneBackgrounds[depth] = bg;
	}

	void Scene::AddSceneBackground(Background* background, int depth)
	{
		SceneBackground bg;

		bg.background = background;
		bg.position = glm::vec3(0, 0, 0);

		m_SceneBackgrounds[depth] = bg;
	}

	void Scene::AddObjectAtPosition(Object& object, int layer, const glm::vec3& position)
	{
		ObjectInstance instance;
		SceneDataItem item;

		string object_id = object.GetObjectID();

		instance.m_Object = &object;
		instance.m_ObjectID = object_id;

		item.ItemObjectInstance = instance;
		item.ItemPos = position;
		item.ItemType = sitem_type_object;
		m_SceneItems[layer][object_id].push_back(item);
	}

	void Scene::UpdateObjectPosition(Object& object, const glm::vec3& position)
	{
		size_t size = 0;
		string id = object.GetObjectID();

		for (int layer_no = 0; layer_no < GLGAME_MAX_LAYERS; layer_no++)
		{
			size = m_SceneItems[layer_no][id].size();

			for (int element = 0; element < size; element++)
			{
				m_SceneItems[layer_no][id][element].ItemPos = position;
			}
		}
	}

	void Scene::IncrementObjectPosition(Object& object, const glm::vec3& increment)
	{
		size_t size = 0;
		string id = object.GetObjectID();

		for (int layer_no = 0; layer_no < GLGAME_MAX_LAYERS; layer_no++)
		{
			size = m_SceneItems[layer_no][id].size();

			for (int element = 0; element < size; element++)
			{
				m_SceneItems[layer_no][id][element].ItemPos.x += increment.x;
				m_SceneItems[layer_no][id][element].ItemPos.y += increment.y;
				m_SceneItems[layer_no][id][element].ItemPos.z += increment.z;
			}
		}
	}

	uint32_t GenerateSceneID()
	{
		static uint32_t scene_id = 00001;

		scene_id += 1;
		return scene_id;
	}
}