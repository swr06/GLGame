#include "Rendering.h"

namespace GLGame
{
	// Used to render backgrounds
	void NormallyRenderBackgrounds(SceneBackground* bg, Shader shader, float w, float h, const glm::mat4& model_matrix, const glm::mat4& view_matrix, const glm::mat4& view_projection_matrix) // camera view is for culling
	{
		static GLfloat* vertex_buffer = new GLfloat[20];
		static GLuint index_buffer[] = { 0,1,2,2,3,0 };

		static VertexBuffer VBO(GL_ARRAY_BUFFER);
		static IndexBuffer IBO;
		static VertexArray VAO;

		array<GLfloat, 8> TextureCoordinates;

		// bug fix : int -> float
		float width;
		float height;

		shader.Use();
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, bg->background->GetTexture().GetTextureID());
		shader.SetInteger("u_Texture", 1, 0);
		shader.SetMatrix4("u_ModelMatrix", model_matrix, 0);
		shader.SetMatrix4("u_ViewProjectionMatrix", view_projection_matrix, 0);

		VAO.Bind();
		IBO.Bind();
		IBO.BufferData(6 * sizeof(GLuint), index_buffer, GL_STATIC_DRAW);
		VBO.VertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
		VBO.VertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
		VAO.Unbind();

		TextureCoordinates = bg->background->GetTexture().GetTextureCoords();
		width = w + bg->position.x;
		height = h + bg->position.y;

		vertex_buffer[0] = width;
		vertex_buffer[1] = bg->position.y;
		vertex_buffer[2] = bg->position.z;
		vertex_buffer[3] = TextureCoordinates[0];
		vertex_buffer[4] = TextureCoordinates[1];

		vertex_buffer[5] = width;
		vertex_buffer[6] = height;
		vertex_buffer[7] = bg->position.z;
		vertex_buffer[8] = TextureCoordinates[2];
		vertex_buffer[9] = TextureCoordinates[3];

		vertex_buffer[10] = bg->position.x;
		vertex_buffer[11] = height;
		vertex_buffer[12] = bg->position.z;
		vertex_buffer[13] = TextureCoordinates[4];
		vertex_buffer[14] = TextureCoordinates[5];

		vertex_buffer[15] = bg->position.x;
		vertex_buffer[16] = bg->position.y;
		vertex_buffer[17] = bg->position.z;
		vertex_buffer[18] = TextureCoordinates[6];
		vertex_buffer[19] = TextureCoordinates[7];

		VAO.Bind();
		VBO.BufferData(20 * sizeof(GLfloat), vertex_buffer, GL_STATIC_DRAW);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)0);

		VAO.Unbind();
	}

	// This function is used by the scene editor 
	void _NormallyRenderSEItems(_RenderItem item, Shader shader, const glm::mat4& projection_matrix)
	{
		static GLfloat* vertex_buffer = new GLfloat[20];
		static GLuint index_buffer[] = { 0,1,3,1,2,3 };

		static VertexBuffer VBO(GL_ARRAY_BUFFER);
		static IndexBuffer IBO;
		static VertexArray VAO;

		array<GLfloat, 8> TextureCoordinates;
		float width;
		float height;

		float w = 0;
		float h = 0;
		float x = 0;
		float y = 0;

		w = item.tw;
		h = item.th;

		shader.Use();
		glActiveTexture(GL_TEXTURE1);
		glBindTexture(GL_TEXTURE_2D, item.tex->GetTextureID());
		shader.SetInteger("u_Texture", 1, 0);
		shader.SetMatrix4("u_ModelMatrix", glm::mat4(1.0f), 0);
		shader.SetMatrix4("u_ViewProjectionMatrix", projection_matrix, 0);

		VAO.Bind();
		IBO.Bind();
		IBO.BufferData(6 * sizeof(GLuint), index_buffer, GL_STATIC_DRAW);
		VBO.VertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)0);
		VBO.VertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
		VAO.Unbind();

		TextureCoordinates = item.tex->GetTextureCoords();
		width = w + item.x;
		height = h + item.y;

		x = item.x;
		y = item.y;
		float z = 1.0f;

		vertex_buffer[0] = width;
		vertex_buffer[1] = y;
		vertex_buffer[2] = z;
		vertex_buffer[3] = TextureCoordinates[0];
		vertex_buffer[4] = TextureCoordinates[1];

		vertex_buffer[5] = width;
		vertex_buffer[6] = height;
		vertex_buffer[7] = z;
		vertex_buffer[8] = TextureCoordinates[2];
		vertex_buffer[9] = TextureCoordinates[3];

		vertex_buffer[10] = x;
		vertex_buffer[11] = height;
		vertex_buffer[12] = z;
		vertex_buffer[13] = TextureCoordinates[4];
		vertex_buffer[14] = TextureCoordinates[5];

		vertex_buffer[15] = x;
		vertex_buffer[16] = y;
		vertex_buffer[17] = z;
		vertex_buffer[18] = TextureCoordinates[6];
		vertex_buffer[19] = TextureCoordinates[7];

		VAO.Bind();
		VBO.BufferData(20 * sizeof(GLfloat), vertex_buffer, GL_STATIC_DRAW);
		glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*)0);

		VAO.Unbind();
	}
}