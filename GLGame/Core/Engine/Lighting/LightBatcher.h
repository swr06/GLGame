#pragma once

#include <GL/glew.h>

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Core\OpenGL Classes\Index Buffer\IndexBuffer.h"
#include "Core\OpenGL Classes\Vertex Array\VertexArray.h"
#include "Core\OpenGL Classes\Vertex Buffer\VertexBuffer.h"
#include "Core\OpenGL Classes\Texture\Texture.h"
#include "Core\OpenGL Classes\Shader\Shader.h"
#include "Core\OpenGL Classes\GLDebug\GLDebug.h"
#include "Core\Engine\Macros\Macros.h"
#include "Core\Engine\Lighting\Lighting.h"

namespace GLGame
{
	class LightBatcher
	{
	public :

		LightBatcher();
		~LightBatcher();
		void AddLightToBatch(Light light);
		void StartLightBatch(const glm::mat4& vp_matrix);

		// Returns the amount of lights that have been drawn
		unsigned int EndLightBatch();

	private : 

		unsigned int DrawFullBatch();

		int m_CurrentElement;
		unsigned int m_VerticesWritten = 0;
		const int m_MaxLights;

		array<GLfloat, 8> m_UVCoords = { -1.0f, 2.0f, -1.0f, -1.0f, 2.0f, -1.0f, 2.0f, 2.0f };

		// Vertex and index data
		GLfloat* m_VertexBuffer;
		GLuint* m_IndexBuffer;

		// OpenGL Abstracted objects
		VertexBuffer m_VBO;
		VertexArray m_VAO;
		IndexBuffer m_IBO;
		Shader m_Shader;

		glm::mat4 m_VPMatrix = glm::mat4(1.0f);
	};

	// This function is INCREDIBLY slow. It was originally only used to test my lighting concept. Please use LightBatcher as it much more fast and reliable
	void DrawLight(Light light);

}