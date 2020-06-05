#include "ParticleBatcher.h"

namespace GLGame
{
	ParticleBatcher::ParticleBatcher() : m_MaxParticles(10000), m_VertexBuffer(nullptr), m_IndexBuffer(nullptr)
	{
		m_VertexBuffer = new GLfloat[28 * m_MaxParticles];
		m_IndexBuffer = new GLuint[6 * m_MaxParticles];
		m_VerticesWritten = 0;
		m_CurrentElement = 0;

		// Generating the index buffer
		int index_offset = 0;

		for (size_t i = 0; i < 6 * m_MaxParticles; i += 6)
		{
			m_IndexBuffer[i] = 0 + index_offset;
			m_IndexBuffer[i + 1] = 1 + index_offset;
			m_IndexBuffer[i + 2] = 3 + index_offset;
			m_IndexBuffer[i + 3] = 1 + index_offset;
			m_IndexBuffer[i + 4] = 2 + index_offset;
			m_IndexBuffer[i + 5] = 3 + index_offset;

			index_offset = index_offset + 4;
		}
	}

	ParticleBatcher::~ParticleBatcher()
	{
		delete[] m_VertexBuffer;
		delete[] m_IndexBuffer;
	}

	void ParticleBatcher::AddParticleToBatch(const glm::mat4& transform, const glm::vec4& color)
	{
		if (m_VerticesWritten == m_MaxParticles)
		{
			DrawFullBatch();
		}

		glm::vec4 v1 = { -0.5f, -0.5f, 0.0f, 1.0f, };
		glm::vec4 v2 = {  0.5f, -0.5f, 0.0f, 1.0f  };
		glm::vec4 v3 = {  0.5f,  0.5f, 0.0f, 1.0f  };
		glm::vec4 v4 = { -0.5f,  0.5f, 0.0f, 1.0f  };

		// Multiplying the vertices
		v1 = transform * v1;
		v2 = transform * v2;
		v3 = transform * v3;
		v4 = transform * v4;
		
		m_VertexBuffer[m_CurrentElement + 0] = v1.x;
		m_VertexBuffer[m_CurrentElement + 1] = v1.y;
		m_VertexBuffer[m_CurrentElement + 2] = v1.z;
		m_VertexBuffer[m_CurrentElement + 3] = color.r;
		m_VertexBuffer[m_CurrentElement + 4] = color.g;
		m_VertexBuffer[m_CurrentElement + 5] = color.b;
		m_VertexBuffer[m_CurrentElement + 6] = color.a;

		m_VertexBuffer[m_CurrentElement + 7]  = v2.x;
		m_VertexBuffer[m_CurrentElement + 8]  = v2.y;
		m_VertexBuffer[m_CurrentElement + 9]  = v2.z;
		m_VertexBuffer[m_CurrentElement + 10] = color.r;
		m_VertexBuffer[m_CurrentElement + 11] = color.g;
		m_VertexBuffer[m_CurrentElement + 12] = color.b;
		m_VertexBuffer[m_CurrentElement + 13] = color.a;

		m_VertexBuffer[m_CurrentElement + 14] = v3.x;
		m_VertexBuffer[m_CurrentElement + 15] = v3.y;
		m_VertexBuffer[m_CurrentElement + 16] = v3.z;
		m_VertexBuffer[m_CurrentElement + 17] = color.r;
		m_VertexBuffer[m_CurrentElement + 18] = color.g;
		m_VertexBuffer[m_CurrentElement + 19] = color.b;
		m_VertexBuffer[m_CurrentElement + 20] = color.a;

		m_VertexBuffer[m_CurrentElement + 21] = v4.x;
		m_VertexBuffer[m_CurrentElement + 22] = v4.y;
		m_VertexBuffer[m_CurrentElement + 23] = v4.z;
		m_VertexBuffer[m_CurrentElement + 24] = color.r;
		m_VertexBuffer[m_CurrentElement + 25] = color.g;
		m_VertexBuffer[m_CurrentElement + 26] = color.b;
		m_VertexBuffer[m_CurrentElement + 27] = color.a;

		m_CurrentElement += 28;
		m_VerticesWritten += 1;
	}

	void ParticleBatcher::StartParticleBatch(const glm::mat4& vp_matrix)
	{
		// Clear the previous batch of lights

		DrawFullBatch();

		m_VPMatrix = vp_matrix;
		m_CurrentElement = 0;
		m_VerticesWritten = 0;
	}

	unsigned int ParticleBatcher::EndParticleBatch()
	{
		return DrawFullBatch();
	}

	unsigned int ParticleBatcher::DrawFullBatch()
	{
		unsigned int ret_val = 0;

		if (!m_Initialized)
		{
			m_VBO = new VertexBuffer(GL_ARRAY_BUFFER),
			m_Shader.CreateShaderProgram(GLGAME_DEFAULT_PARTICLE_VERTEX, GLGAME_DEFAULT_PARTICLE_FRAGMENT);
			m_VAO.Bind();

			m_IBO.BufferData(6 * m_MaxParticles * sizeof(GLuint), m_IndexBuffer, GL_STATIC_DRAW);

			// Position attribute
			m_VBO->VertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), (GLvoid*)0);

			// Color attribute
			m_VBO->VertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 7 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
			m_Initialized = true;
		}

		m_Shader.Use();

		m_Shader.SetMatrix4("u_ViewProjectionMatrix", m_VPMatrix, 0);
		m_VAO.Bind();
		m_VBO->BufferData((m_VerticesWritten * 28) * sizeof(GLfloat), m_VertexBuffer, GL_STATIC_DRAW);
		glDrawElements(GL_TRIANGLES, m_VerticesWritten * 6, GL_UNSIGNED_INT, (void*)0);
		m_VAO.Unbind();

		ret_val = m_VerticesWritten;
		m_CurrentElement = 0;
		m_VerticesWritten = 0;

		return ret_val;
	}
}