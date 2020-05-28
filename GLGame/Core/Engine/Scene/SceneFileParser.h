#pragma once

#include <iostream>
#include <fstream>

#include "Core\OpenGL Classes\GLDebug\GLDebug.h"
#include "Core\Engine\Scene\Scene.h"
#include "Core\Engine\Scene\SceneData.h"
#include "Core\Engine\Macros\Macros.h"

namespace GLGame
{
	namespace GameInternal
	{
		Object* _GetObjectFromGlobalArray(const string& id);
	}

	namespace SceneParser
	{
		enum SceneParsedItemType
		{
			ItemTypeObject,
			ItemTypeSprite,
			ItemTypeTex,
		};

		struct SceneParsedDataItem
		{
			SceneParsedItemType type;
			string id;
			float x;
			float y;
			int sx;
			int sy;
			int layer;
		};

		static std::ifstream::pos_type GetFileSize(const string& filename);
		void ParseSceneData(const string& scene_file_pth, Scene* scene);
		void _FillInParsedSceneData(vector<SceneParsedDataItem>& items, Scene* scene);
		void _ParseSceneFileData(Scene* scene, const string& scene_file);
	}
}