#include "LightBatcher.h"

namespace GLGame
{
	void DrawLight(Light light)
	{
		GLfloat* vertex_buffer = new GLfloat[35 * 4];
		GLuint index_buffer[] = { 0,1,2,2,3,0 };

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
		light_shader.CreateShaderProgram(GLGAME_DEFAULT_LIGHT_VERTEX, GLGAME_DEFAULT_LIGHT_FRAGMENT);
		light_shader.Use();
		light_shader.SetMatrix4("u_ViewProjectionMatrix", projection_matrix, 0);

		// Filling the vertex buffer

		float x = light.m_Position.x - light.m_Diameter / 2;
		float y = light.m_Position.y - light.m_Diameter / 2;

		float width = light.m_Diameter + x;
		float height = light.m_Diameter + y;

		array<GLfloat, 8> UVCoords = {  -1.0f, 2.0f, -1.0f, -1.0f, 2.0f, -1.0f, 2.0f, 2.0f };
	
		vertex_buffer[0] = width;
		vertex_buffer[1] = y;
		vertex_buffer[2] = light.m_Position.z;
		vertex_buffer[3] = light.m_Color.r;
		vertex_buffer[4] = light.m_Color.g;
		vertex_buffer[5] = light.m_Color.b;
		vertex_buffer[6] = light.m_Color.a;
		vertex_buffer[7] = UVCoords[0];
		vertex_buffer[8] = UVCoords[1];

		vertex_buffer[9]  =  width;
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
		VBO.BufferData(35 * sizeof(GLfloat), vertex_buffer, GL_STATIC_DRAW);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)0);
		VAO.Unbind();
	}
}