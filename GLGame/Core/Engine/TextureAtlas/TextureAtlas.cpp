#include "TextureAtlas.h"

namespace GLGame
{
	TextureAtlas::TextureAtlas(string atlas_path, array<uint32_t, 2> atlas_tile_size)
		: m_AtlasSizeX(0), m_AtlasSizeY(0), m_TileCount(0)
	{
		auto start = chrono::steady_clock::now();

		Texture* tex = new Texture;

		tex->CreateTexture(atlas_path);
		m_Atlas = tex;
		m_SingleTileSize = atlas_tile_size;

		auto end = chrono::steady_clock::now();
		double elapsed_time = std::chrono::duration_cast<std::chrono::duration<double>>(end - start).count();

		Log::_LogTextureAtlasCreation(glfwGetTime(), elapsed_time, tex->GetTextureID(), atlas_path, __FILE__, __LINE__);
	}

	TextureAtlas::TextureAtlas(Texture& atlas) : m_SingleTileSize({ 0,0 }), m_AtlasSizeX(0), m_AtlasSizeY(0), m_TileCount(0)
	{
		auto start = chrono::steady_clock::now();

		m_Atlas= &atlas;

		auto end = chrono::steady_clock::now();
		double elapsed_time = std::chrono::duration_cast<std::chrono::duration<double>>(end - start).count();

		Log::_LogTextureAtlasCreation(glfwGetTime(), elapsed_time, atlas.GetTextureID(), atlas.GetTexturePath(), __FILE__, __LINE__);
	}

	Texture* TextureAtlas::SampleTexture(array<GLfloat, 4> texture_coords, bool convert)
	{
		Texture* tex = new Texture;
		*tex = *m_Atlas;

		const short x1 = 0; 
		const short y1 = 1; 
		const short x2 = 2; 
		const short y2 = 3; 
		array<GLfloat, 8> texture_coordinates;

		if (convert)
		{
			texture_coords[0] = ConvertValue(texture_coords[0], 0, m_Atlas->GetWidth(), -1, 1);
			texture_coords[1] = ConvertValue(texture_coords[1], 0, m_Atlas->GetHeight(), -1, 1);
			texture_coords[2] = ConvertValue(texture_coords[2], 0, m_Atlas->GetWidth(), -1, 1);
			texture_coords[3] = ConvertValue(texture_coords[3], 0, m_Atlas->GetHeight(), -1, 1);
		}

		texture_coordinates[0] = texture_coords[x1]; 
		texture_coordinates[1] = texture_coords[y2]; 
		texture_coordinates[2] = texture_coords[x2]; 
		texture_coordinates[3] = texture_coords[y2]; 
		texture_coordinates[4] = texture_coords[x2]; 
		texture_coordinates[5] = texture_coords[y1]; 
		texture_coordinates[6] = texture_coords[x1]; 
		texture_coordinates[7] = texture_coords[y1]; 

		tex->IntCreateTexture(m_Atlas->GetTextureID(), texture_coordinates, 64,64, false); 
		tex->m_delete_texture = false;
		return tex;
	}

	Texture* TextureAtlas::SampleTexture_(array<GLfloat, 8> texture_coords, bool convert)
	{
		Texture* tex = new Texture;

		*tex = *m_Atlas;

		if (convert)
		{
			texture_coords[0] = ConvertValue(texture_coords[0], 0, m_Atlas->GetWidth(), -1.0f, 1.0f);
			texture_coords[1] = ConvertValue(texture_coords[1], 0, m_Atlas->GetHeight(), -1.0f, 1.0f);
			texture_coords[2] = ConvertValue(texture_coords[2], 0, m_Atlas->GetWidth(), -1.0f, 1.0f);
			texture_coords[3] = ConvertValue(texture_coords[3], 0, m_Atlas->GetHeight(), -1.0f, 1.0f);
			texture_coords[4] = ConvertValue(texture_coords[4], 0, m_Atlas->GetWidth(), -1.0f, 1.0f);
			texture_coords[5] = ConvertValue(texture_coords[5], 0, m_Atlas->GetHeight(), -1.0f, 1.0f);
			texture_coords[6] = ConvertValue(texture_coords[6], 0, m_Atlas->GetWidth(), -1.0f, 1.0f);
			texture_coords[7] = ConvertValue(texture_coords[7], 0, m_Atlas->GetHeight(), -1.0f, 1.0f);
		}

		tex->IntCreateTexture(m_Atlas->GetTextureID(), texture_coords, 64, 64, false);
		return tex;
	}

	uint32_t TextureAtlas::GetAmountVerticalTiles()
	{
		if (m_SingleTileSize[0] > 0)
		{
			return m_Atlas->GetWidth() / m_SingleTileSize[0];
		}
		
		else
		{
			// Error! To be logged
			return 0; 
		}
	}

	uint32_t TextureAtlas::GetAmountHorizontalTiles()
	{
		if (m_SingleTileSize[1] > 0)
		{
			return m_Atlas->GetHeight() / m_SingleTileSize[1];
		}

		else
		{
			// Error! To be logged
			return 0;
		}
	}

	uint32_t TextureAtlas::GetAmountTiles()
	{
		if (m_SingleTileSize[0] > 0 && m_SingleTileSize[1] > 0)
		{
			return m_Atlas->GetWidth() / m_SingleTileSize[0] +
				m_Atlas->GetHeight() / m_SingleTileSize[1];
		}

		else
		{
			return 0;
		}
	}

	float TextureAtlas::ConvertValue(float value, float old_min, float old_max, float new_min, float new_max)
	{
		float old_range;
		float new_range;
		float ret_val;

		old_range = (old_max - old_min);

		if (old_range == 0)
		{
			ret_val = new_min;
		}
			
		else
		{
			new_range = (new_max - new_min);
			ret_val = (((value - old_min) * new_range) / old_range) + new_min;
		}

		return ret_val;
	}
}

