#include "Animation.h"

namespace GLGame
{
	Animation::Animation(vector<Texture*> Textures)
	{
		for (int i = 0; i < Textures.size(); i++)
		{
			m_Textures.push_back(Textures[i]);
		}

		Textures.clear();
	}

	Animation::Animation(vector<std::string> TexturePaths)
	{
		for (int i = 0; i < TexturePaths.size(); i++)
		{
			Texture* tex = new Texture;
			tex->CreateTexture(TexturePaths[i]);
			m_Textures.push_back(tex);

			tex = nullptr;
		}
	}

	pair<int, Texture*> Animation::GetFrame(int curr_frame)
	{
		pair<int, Texture*> ret_val;

		if (curr_frame >= m_Textures.size())
		{
			ret_val = { 0, m_Textures[0] };
		}

		else
		{
			ret_val = { curr_frame, m_Textures[curr_frame] };
		}

		return ret_val;
	}

	void Animation::AddFrame(Texture& Tex)
	{
		m_Textures.push_back(&Tex);
	}
}
