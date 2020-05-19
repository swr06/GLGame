#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <vector>
#include <string>

#include "Core\OpenGL Classes\GLDebug\GLDebug.h"
#include "Core\OpenGL Classes\Texture\Texture.h"
#include "Core\Engine\Logger\Log.h"

namespace GLGame
{
	using namespace std;

	class TextureAtlas
	{
	public : 

		TextureAtlas(string atlas_path, array<uint32_t, 2> atlas_tile_size);
		TextureAtlas(Texture & atlas);
		// ToDo : Texture GetTile() ; 

		Texture* SampleTexture(array<GLfloat, 4> texture_coords, bool convert); // convert tells whether the function has to convert the coordinates
		Texture* SampleTexture_(array<GLfloat, 8> texture_coords, bool convert);
		uint32_t GetAmountVerticalTiles();
		uint32_t GetAmountHorizontalTiles();
		uint32_t GetAmountTiles();

	private :

		//uint32_t ConvertValue(uint32_t value, uint32_t old_min, uint32_t old_max, uint32_t new_min, uint32_t new_max);
		float ConvertValue(float value, float old_min, float old_max, float new_min, float new_max); 

		Texture* m_Atlas;
		vector<Texture> m_TextureLookup;
		string m_AtlasPath;
		uint32_t m_TileCount;
		array<uint32_t, 2> m_SingleTileSize;
		int m_AtlasSizeX;
		int m_AtlasSizeY;
	};
}