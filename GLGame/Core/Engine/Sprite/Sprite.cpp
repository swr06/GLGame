#include "Sprite.h" 

// UwU

namespace GLGame
{
	Sprite::Sprite(const string& id, Texture& tex) : m_Animation(nullptr), m_Texture(nullptr), m_AnimatedSprite(0), m_Frequency(1), m_CurrentTexture(nullptr)
	{
		auto start = chrono::steady_clock::now();

		const char* spr_append = "$*$^(\0";

		m_ID = id;
		m_ID.insert(0, spr_append);
		m_Texture = &tex;
		m_DisplayID = id;
		m_AnimatedSprite = false;
		m_CurrentTexture = m_Texture;
		GameInternal::_IntRegisterSprite(this);

		auto end = chrono::steady_clock::now();
		double elapsed_time = std::chrono::duration_cast<std::chrono::duration<double>>(end - start).count();

		Log::_LogSpriteCreation(glfwGetTime(), elapsed_time, m_DisplayID, tex.GetTexturePath(), __FILE__, __LINE__);
	}

	Sprite::Sprite(const string& id, Texture* tex) : m_Animation(nullptr), m_Texture(nullptr), m_AnimatedSprite(0), m_Frequency(1), m_CurrentTexture(nullptr)
	{
		auto start = chrono::steady_clock::now();

		const char* spr_append = "$*$^(\0";

		m_ID = id;
		m_ID.insert(0, spr_append);
		m_DisplayID = id;
		m_Texture = tex;
		m_AnimatedSprite = false;
		m_CurrentTexture = m_Texture;
		GameInternal::_IntRegisterSprite(this);

		auto end = chrono::steady_clock::now();
		double elapsed_time = std::chrono::duration_cast<std::chrono::duration<double>>(end - start).count();

		if (tex != nullptr)
		{
			Log::_LogSpriteCreation(glfwGetTime(), elapsed_time, m_DisplayID, tex->GetTexturePath(), __FILE__, __LINE__);
		}

		else
		{
			Log::_LogSpriteCreation(glfwGetTime(), elapsed_time, m_DisplayID, "TEX_ERR", __FILE__, __LINE__);
		}
	}

	Sprite::Sprite(const string& id, Animation& animation, size_t speed) : m_Animation(nullptr), m_Texture(nullptr), m_AnimatedSprite(1), m_CurrentTexture(nullptr)
	{
		auto start = chrono::steady_clock::now();
		std::pair<int, Texture*> ani_tex = animation.GetFrame(0);

		const char* spr_append = "$*$^(\0";

		m_ID = id;
		m_ID.insert(0, spr_append);
		m_DisplayID = id;
		m_Frequency = speed;
		m_Animation = &animation;
		m_AnimatedSprite = true;
		m_CurrentTexture = ani_tex.second;
		GameInternal::_IntRegisterSprite(this);

		auto end = chrono::steady_clock::now();
		double elapsed_time = std::chrono::duration_cast<std::chrono::duration<double>>(end - start).count();

		Log::_LogSpriteCreation(glfwGetTime(), elapsed_time, m_DisplayID, "Animation paths", __FILE__, __LINE__);
	}

	Sprite::~Sprite()
	{
		GameInternal::_IntDeregisterSprite(this);
	}

	void Sprite::UpdateSprite(long long fps)
	{
		pair<int, Texture*> val;

		if (m_AnimatedSprite)
		{
			if (fps % m_Frequency == 0)
			{
				val = m_Animation->GetFrame(m_CurrentFrame + 1);
				m_CurrentFrame = val.first;
				m_CurrentTexture = val.second;
			}
		}
	}
}