#pragma once

#include "Core\Engine\Macros\Macros.h"
#include "Core\OpenGL Classes\Index Buffer\IndexBuffer.h"
#include "Core\OpenGL Classes\Vertex Array\VertexArray.h"
#include "Core\OpenGL Classes\Vertex Buffer\VertexBuffer.h"
#include "Core\OpenGL Classes\Texture\Texture.h"
#include "Core\OpenGL Classes\Shader\Shader.h"

namespace GLGame
{
	class ParticleBatcher
	{
	public:

		ParticleBatcher();
		~ParticleBatcher();
		void AddParticleToBatch(const glm::mat4& transform, const glm::vec4& color);
		void StartParticleBatch(const glm::mat4& vp_matrix);
		void EndParticleBatch();

	private:

		void DrawFullBatch();

		int m_CurrentElement;
		int m_VerticesWritten;
		bool m_Initialized = false;
		const int m_MaxParticles;

		array<GLfloat, 8> m_UVCoords = { -1.0f, 2.0f, -1.0f, -1.0f, 2.0f, -1.0f, 2.0f, 2.0f };

		// Vertex and index data
		GLfloat* m_VertexBuffer;
		GLuint* m_IndexBuffer;

		// OpenGL Abstracted objects
		VertexBuffer* m_VBO;
		VertexArray m_VAO;
		IndexBuffer m_IBO;
		Shader m_Shader;

		glm::mat4 m_VPMatrix = glm::mat4(1.0f);
	};
}