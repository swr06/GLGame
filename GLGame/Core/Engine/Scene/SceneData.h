#pragma once

#include "Core\Engine\Background\Background.h"
#include "Core\Engine\Camera\Camera.h"
#include "Core\Engine\Instance\Instance.h"

#include <iostream>
#include <vector>
#include <string>
#include <map>

namespace GLGame
{
	using namespace std;

	enum SceneDataItemType
	{
		sitem_type_background,
		sitem_type_object,
		sitem_type_sprite,
		sitem_type_atlas_sample,
		sitem_type_sound,
	};

	struct SceneDataItem
	{
		SceneDataItemType ItemType;
		uint32_t ItemID; // Instance ID
		glm::vec3 ItemPos;
		std::pair<int, int> ItemPosAtlasSampleCoords;

		// Instance structures
		ObjectInstance ItemObjectInstance;
		SpriteInstance ItemSpriteInstance;
		TextureInstance ItemTextureInstance;
	};

	struct SceneData
	{
		vector<SceneDataItem> SceneObjectDataID;
		string SceneDataPath;
	};

	struct SceneBackground
	{
		Background* background;
		glm::vec3 position;
	};
}