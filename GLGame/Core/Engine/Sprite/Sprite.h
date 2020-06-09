#pragma once

#include <GL/glew.h>
#include <GLFW\glfw3.h>

#include <chrono>

#include "Core\OpenGL Classes\GLDebug\GLDebug.h"
#include "Core\OpenGL Classes\Texture\Texture.h"
#include "Core\OpenGL Classes\Shader\Shader.h"
#include "Core\Engine\Animation\Animation.h"
#include "Core\Engine\Logger\Log.h"

namespace GLGame
{
	// Not used, I decided that the ID should be a string
	static uint32_t GenerateSpriteID()
	{
		static uint32_t spr_id = 00001;

		spr_id += 1;
		return spr_id;
	}

	class Sprite;

	namespace GameInternal
	{
		void _IntRegisterSprite(Sprite* sprite);
		void _IntDeregisterSprite(Sprite* sprite);
	}

	class Sprite
	{
	public : 

		Sprite(const string& id, Texture& tex);
		Sprite(const string& id, Texture* tex);
		Sprite(const string& id, Animation& animation, size_t speed);
		~Sprite();

		void UpdateSprite(long long fps);

		// Do not use this function. Use the GetSpriteDisplayID() function instead. The id has some extra added characters at the beginning
		const string& GetSpriteID() const { return m_ID; }
		Texture* GetCurrentTexture() const { return m_CurrentTexture; }

		Animation* GetSpriteAnimation() const 
		{
			if (m_AnimatedSprite)
			{
				return m_Animation;
			}

			else
			{
				// Error!
				return nullptr; 
			}
		}

		int GetCurrentTextureWidth() const 
		{ 
			if (m_AnimatedSprite)
			{
				return m_CurrentTexture->GetWidth();
			}

			else
			{
				return m_Texture->GetWidth();
			}
		}

		int GetCurrentTextureHeight() const 
		{ 
			if (m_AnimatedSprite)
			{
				return m_CurrentTexture->GetHeight();
			}

			else
			{
				return m_Texture->GetHeight();
			}
		}

	private : 

		Animation* m_Animation; 
		Texture* m_Texture;
		Texture* m_CurrentTexture; 
		size_t m_Frequency = 1;
		string m_ID; 
		string m_DisplayID;
		int m_CurrentFrame = 0;

		bool m_AnimatedSprite;
	};
}