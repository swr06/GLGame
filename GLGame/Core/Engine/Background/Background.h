#pragma once

#include <GL/glew.h>
#include <array>
#include <string>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Core\OpenGL Classes\GLDebug\GLDebug.h"
#include "Core\OpenGL Classes\Texture\Texture.h"
#include "Core\OpenGL Classes\Shader\Shader.h"
#include "Core\Engine\Defaults\Defaults.h"
#include "Core\Engine\Macros\Macros.h"

namespace GLGame
{
	using namespace std;

	class Background;

	namespace GameInternal
	{
		void _IntRegisterBackground(Background* bg);
		void _IntDeregisterBackground(Background* bg);
		void _SetBackgroundPosition(const string& id, const glm::vec3& pos);
		void _IncrementBackgroundPosition(const string& id, const glm::vec3& increment);
	}

	class Background
	{
	public:

		Background(const string& background, const string& bg_id, bool stretch_to_window = true, Shader* shader = nullptr, GLenum repeat_type_s = GL_REPEAT,
			GLenum repeat_type_t = GL_REPEAT, GLenum min_filter = GL_NEAREST,
			GLenum mag_filter = GL_NEAREST,
			array<GLfloat, 8> texture_sampler_coordinates = { 1.0f, 1.0f,  1.0f, 0.0f, 0.0f, 0.0f, 0.0f, 1.0f });
		~Background();

		void SetShader(Shader& shader);
		Shader* GetShader() { return m_Shader; }

		void ApplyRotationTransformation(float angle);
		void ApplyScaleTransformation(glm::vec3 scale_factor);
		void SetPosition(const glm::vec3 position) const;
		void SetStretchToWindow(bool val) { m_StretchToWindow = val; }
		void IncrementPosition(const glm::vec3 increment) const;
		const glm::mat4& GetModelMatrix() const { return m_ModelMatrix; }
		const Texture& GetTexture() { return *m_Background; }
		bool MovesWithCamera() { return m_MovesWithCamera; }
		bool ShouldStretchToWindow() { return m_StretchToWindow; }
		void SetMovesWithCamera(bool flag);
		string GetBackgroundID() const { return m_ID; }

	private:

		bool m_MovesWithCamera = false;
		bool m_StretchToWindow = false;
		uint32_t GenerateBackgroundID();

		glm::mat4 m_ModelMatrix;
		Texture* m_Background;
		GLenum m_RepeatTypeS;
		GLenum m_RepeatTypeT;
		Shader* m_Shader = nullptr;
		string m_ID;
	};
}