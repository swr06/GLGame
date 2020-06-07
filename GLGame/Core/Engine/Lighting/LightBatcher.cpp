#include "LightBatcher.h"

namespace GLGame
{
	// This function is INCREDIBLY slow. It was originally only used to test my lighting concept. Please use LightBatcher
	void DrawLight(Light light)
	{

		GLfloat* vertex_buffer = new GLfloat[36];
		GLuint index_buffer[] = { 0,1,3,1,2,3 };

		// OpenGL abstracted variables
		Shader light_shader;
		VertexBuffer VBO(GL_ARRAY_BUFFER);
		IndexBuffer IBO;
		VertexArray VAO;

		glm::mat4 projection_matrix = glm::ortho(0.0f, 800.0f, 0.0f, 600.0f);

		VAO.Bind();
		IBO.BufferData(6 * sizeof(GLuint), index_buffer, GL_STATIC_DRAW);

		// Position attribute
		VBO.VertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), (GLvoid*)0);

		// Color attribute
		VBO.VertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));

		// UV Coordinates
		VBO.VertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), (GLvoid*)(7 * sizeof(GLfloat)));
		VAO.Unbind();

		// Setup the light shader
		light_shader.CreateShaderProgramFromFile(GLGAME_DEFAULT_LIGHT_VERTEX, GLGAME_DEFAULT_LIGHT_FRAGMENT);
		light_shader.Use();
		light_shader.SetMatrix4("u_ViewProjectionMatrix", projection_matrix, 0);

		// Filling the vertex buffer

		float x = light.m_Position.x - light.m_Size;
		float y = light.m_Position.y - light.m_Size;

		float width = light.m_Size + x;
		float height = light.m_Size + y;

		array<GLfloat, 8> UVCoords = { -1.0f, 2.0f, -1.0f, -1.0f, 2.0f, -1.0f, 2.0f, 2.0f };

		vertex_buffer[0] = width;
		vertex_buffer[1] = y;
		vertex_buffer[2] = light.m_Position.z;
		vertex_buffer[3] = light.m_Color.r;
		vertex_buffer[4] = light.m_Color.g;
		vertex_buffer[5] = light.m_Color.b;
		vertex_buffer[6] = light.m_Color.a;
		vertex_buffer[7] = UVCoords[0];
		vertex_buffer[8] = UVCoords[1];

		vertex_buffer[9] = width;
		vertex_buffer[10] = height;
		vertex_buffer[11] = light.m_Position.z;
		vertex_buffer[12] = light.m_Color.r;
		vertex_buffer[13] = light.m_Color.g;
		vertex_buffer[14] = light.m_Color.b;
		vertex_buffer[15] = light.m_Color.a;
		vertex_buffer[16] = UVCoords[2];
		vertex_buffer[17] = UVCoords[3];

		vertex_buffer[18] = x;
		vertex_buffer[19] = height;
		vertex_buffer[20] = light.m_Position.z;
		vertex_buffer[21] = light.m_Color.r;
		vertex_buffer[22] = light.m_Color.g;
		vertex_buffer[23] = light.m_Color.b;
		vertex_buffer[24] = light.m_Color.a;
		vertex_buffer[25] = UVCoords[4];
		vertex_buffer[26] = UVCoords[5];

		vertex_buffer[27] = x;
		vertex_buffer[28] = y;
		vertex_buffer[29] = light.m_Position.z;
		vertex_buffer[30] = light.m_Color.r;
		vertex_buffer[31] = light.m_Color.g;
		vertex_buffer[32] = light.m_Color.b;
		vertex_buffer[33] = light.m_Color.a;
		vertex_buffer[34] = UVCoords[6];
		vertex_buffer[35] = UVCoords[7];

		// Draw the triangles
		VAO.Bind();
		VBO.BufferData(36 * sizeof(GLfloat), vertex_buffer, GL_STATIC_DRAW);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)0);
		VAO.Unbind();
	}

	LightBatcher::LightBatcher() : m_VBO(GL_ARRAY_BUFFER), m_MaxLights(512), m_VertexBuffer(nullptr), m_IndexBuffer(nullptr)
	{
		m_ObjectsInitialized = false;
		m_VertexBuffer = new GLfloat[35 * m_MaxLights];
		m_IndexBuffer = new GLuint[6 * m_MaxLights];
		m_VerticesWritten = 0;
		m_CurrentElement = 0;

		// Generating the index buffer
		int index_offset = 0;

		for (size_t i = 0; i < 6 * m_MaxLights; i += 6)
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

	LightBatcher::~LightBatcher()
	{
		delete[] m_VertexBuffer;
		delete[] m_IndexBuffer;
	}

	void LightBatcher::AddLightToBatch(Light light)
	{
		if (m_VerticesWritten == m_MaxLights)
		{
			DrawFullBatch();
		}

		float x = light.m_Position.x - light.m_Size;
		float y = light.m_Position.y - light.m_Size;

		float width = light.m_Size + x;
		float height = light.m_Size + y;

		m_VertexBuffer[m_CurrentElement + 0] = width;
		m_VertexBuffer[m_CurrentElement + 1] = y;
		m_VertexBuffer[m_CurrentElement + 2] = light.m_Position.z;
		m_VertexBuffer[m_CurrentElement + 3] = light.m_Color.r;
		m_VertexBuffer[m_CurrentElement + 4] = light.m_Color.g;
		m_VertexBuffer[m_CurrentElement + 5] = light.m_Color.b;
		m_VertexBuffer[m_CurrentElement + 6] = light.m_Color.a;
		m_VertexBuffer[m_CurrentElement + 7] = m_UVCoords[0];
		m_VertexBuffer[m_CurrentElement + 8] = m_UVCoords[1];

		m_VertexBuffer[m_CurrentElement + 9] = width;
		m_VertexBuffer[m_CurrentElement + 10] = height;
		m_VertexBuffer[m_CurrentElement + 11] = light.m_Position.z;
		m_VertexBuffer[m_CurrentElement + 12] = light.m_Color.r;
		m_VertexBuffer[m_CurrentElement + 13] = light.m_Color.g;
		m_VertexBuffer[m_CurrentElement + 14] = light.m_Color.b;
		m_VertexBuffer[m_CurrentElement + 15] = light.m_Color.a;
		m_VertexBuffer[m_CurrentElement + 16] = m_UVCoords[2];
		m_VertexBuffer[m_CurrentElement + 17] = m_UVCoords[3];

		m_VertexBuffer[m_CurrentElement + 18] = x;
		m_VertexBuffer[m_CurrentElement + 19] = height;
		m_VertexBuffer[m_CurrentElement + 20] = light.m_Position.z;
		m_VertexBuffer[m_CurrentElement + 21] = light.m_Color.r;
		m_VertexBuffer[m_CurrentElement + 22] = light.m_Color.g;
		m_VertexBuffer[m_CurrentElement + 23] = light.m_Color.b;
		m_VertexBuffer[m_CurrentElement + 24] = light.m_Color.a;
		m_VertexBuffer[m_CurrentElement + 25] = m_UVCoords[4];
		m_VertexBuffer[m_CurrentElement + 26] = m_UVCoords[5];

		m_VertexBuffer[m_CurrentElement + 27] = x;
		m_VertexBuffer[m_CurrentElement + 28] = y;
		m_VertexBuffer[m_CurrentElement + 29] = light.m_Position.z;
		m_VertexBuffer[m_CurrentElement + 30] = light.m_Color.r;
		m_VertexBuffer[m_CurrentElement + 31] = light.m_Color.g;
		m_VertexBuffer[m_CurrentElement + 32] = light.m_Color.b;
		m_VertexBuffer[m_CurrentElement + 33] = light.m_Color.a;
		m_VertexBuffer[m_CurrentElement + 34] = m_UVCoords[6];
		m_VertexBuffer[m_CurrentElement + 35] = m_UVCoords[7];

		m_CurrentElement += 36;
		m_VerticesWritten += 1;
	}

	void LightBatcher::StartLightBatch(const glm::mat4& vp_matrix)
	{
		// Clear the previous batch of lights

		DrawFullBatch();

		m_VPMatrix = vp_matrix;
		m_CurrentElement = 0;
		m_VerticesWritten = 0;
	}

	unsigned int LightBatcher::EndLightBatch()
	{
		if (!m_ObjectsInitialized)
		{
			m_Shader.CreateShaderProgramFromFile(GLGAME_DEFAULT_LIGHT_VERTEX, GLGAME_DEFAULT_LIGHT_FRAGMENT);
			m_VAO.Bind();

			m_IBO.BufferData(6 * m_MaxLights * sizeof(GLuint), m_IndexBuffer, GL_STATIC_DRAW);
			// Position attribute
			m_VBO.VertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), (GLvoid*)0);

			// Color attribute
			m_VBO.VertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));

			// UV Coordinates
			m_VBO.VertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 9 * sizeof(GLfloat), (GLvoid*)(7 * sizeof(GLfloat)));
			m_VAO.Unbind();

			m_ObjectsInitialized = true;
		}

		return DrawFullBatch();
	}

	unsigned int LightBatcher::DrawFullBatch()
	{
		unsigned int ret_val = 0;

		m_Shader.Use();

		m_Shader.SetMatrix4("u_ViewProjectionMatrix", m_VPMatrix, 0);
		m_VAO.Bind();
		m_VBO.BufferData((m_VerticesWritten * 36) * sizeof(GLfloat), m_VertexBuffer, GL_STATIC_DRAW);
		glDrawElements(GL_TRIANGLES, m_VerticesWritten * 6, GL_UNSIGNED_INT, (void*)0);
		m_VAO.Unbind();

		ret_val = m_VerticesWritten;
		m_CurrentElement = 0;
		m_VerticesWritten = 0;

		return ret_val;
	}
}