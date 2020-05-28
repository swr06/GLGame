#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Core\OpenGL Classes\GLDebug\GLDebug.h"

#include "Core\Engine\Collision\AABB.h"
#include "Core\Engine\Object\Object.h"
#include "Core\Engine\Sprite\Sprite.h"
#include "Core\OpenGL Classes\Texture\Texture.h"

namespace GLGame
{
	class ObjectInstance
	{
	public :

		Object* m_Object; 
		string m_ObjectID; 
	};

	class SpriteInstance
	{
	public:

		Sprite* m_Sprite;
		string m_SpriteID;
	};

	class TextureInstance
	{
	public:

		Texture* m_Tex;
		uint32_t m_TexID;
	};
}