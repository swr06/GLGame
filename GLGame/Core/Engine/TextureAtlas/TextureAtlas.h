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

		TextureAtlas(Texture* atlas_texture, int tx, int ty)
		{
			m_Atlas = atlas_texture;
			m_TileX = tx;
			m_TileY = ty;
		}

		Texture* Sample(const glm::vec2& start_coords, const glm::vec2& end_coords)
		{
			float width, height;

			width = m_TileX * (end_coords.x - start_coords.x);
			height = m_TileY * (end_coords.y - start_coords.y);
		
			array<GLfloat, 8> TextureCoordinates;

			TextureCoordinates[0] = (end_coords.x * m_TileX) / m_Atlas->GetHeight();
			TextureCoordinates[1] = (end_coords.y * m_TileY) / m_Atlas->GetWidth();

			TextureCoordinates[2] = (end_coords.x * m_TileX) / m_Atlas->GetWidth();
			TextureCoordinates[3] = (start_coords.y * m_TileY) / m_Atlas->GetHeight();

			TextureCoordinates[4] = (start_coords.x * m_TileX) / m_Atlas->GetWidth();
			TextureCoordinates[5] = (start_coords.y * m_TileY) / m_Atlas->GetHeight();

			TextureCoordinates[6] = (start_coords.x * m_TileX) / m_Atlas->GetWidth();
			TextureCoordinates[7] = (end_coords.y * m_TileY) / m_Atlas->GetHeight();

			Texture *tex = new Texture;

			*tex = *m_Atlas;
			tex->IntCreateTexture(m_Atlas->GetTextureID(), TextureCoordinates, width, height, false);

			return tex;
		}

	private : 

		int m_TileX, m_TileY;
		Texture* m_Atlas;
	};
}