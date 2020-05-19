#pragma once

#include <vector>
#include <iostream>

#include "Core\OpenGL Classes\GLDebug\GLDebug.h"

#include "Core\OpenGL Classes\Texture\Texture.h"
#include "Core\OpenGL Classes\Shader\Shader.h"

namespace GLGame
{
	class Animation
	{
	public : 
		Animation(vector<Texture*> Textures);
		Animation(vector<std::string> TexturePaths);
		pair<int, Texture*> GetFrame(int curr_frame); 
		void AddFrame(Texture& Tex);

	private :

		vector<Texture*> m_Textures;    
	};

}