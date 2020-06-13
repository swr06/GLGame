#include "Background.h"

namespace GLGame
{
	Background::Background(const string& background, const string& bg_id, bool stretch_to_window, Shader* shader, GLenum repeat_type_s, GLenum repeat_type_t, GLenum min_filter, GLenum mag_filter, array<GLfloat, 8> texture_sampler_coordinates)
		: m_Background(nullptr), m_RepeatTypeS(GL_REPEAT), m_RepeatTypeT(GL_REPEAT), m_Shader(nullptr)
	{
		m_StretchToWindow = stretch_to_window;

		if (shader != nullptr)
		{
			m_Shader = shader;
		}

		else
		{
			m_Shader = nullptr;
		}
		
		m_Background = new Texture;

		m_Background->CreateTexture(background, GL_RGBA, GL_TEXTURE_2D,
			min_filter, mag_filter, repeat_type_s, repeat_type_t, texture_sampler_coordinates, true);

		m_RepeatTypeS = repeat_type_s;
		m_RepeatTypeT = repeat_type_t;

		m_ID = bg_id;
		m_ModelMatrix = glm::mat4(1.0f);
		GameInternal::_IntRegisterBackground(this);
	}

	Background::~Background()
	{
		GameInternal::_IntDeregisterBackground(this);
	}

	void Background::SetShader(Shader& shader)
	{
		m_Shader = &shader;
	}

	void Background::ApplyRotationTransformation(float angle)
	{
		m_ModelMatrix = glm::rotate(glm::mat4(m_ModelMatrix), glm::radians(angle), glm::vec3(0.0f, 0.0f, 1.0f));
	}

	void Background::ApplyScaleTransformation(glm::vec3 scale_factor)
	{
		m_ModelMatrix = glm::scale(glm::mat4(m_ModelMatrix), scale_factor);
	}

	void Background::SetPosition(const glm::vec3 position) const
	{
		GameInternal::_SetBackgroundPosition(this->m_ID, position);
	}

	void Background::IncrementPosition(const glm::vec3 increment) const
	{
		GameInternal::_IncrementBackgroundPosition(this->m_ID, increment);
	}

	void Background::SetMovesWithCamera(bool flag)
	{
		m_MovesWithCamera = flag;
	}

	uint32_t Background::GenerateBackgroundID()
	{
		static uint32_t last_id = 00001;

		last_id++;
		return last_id;
	}
}