#pragma once

#include <GL/glew.h>

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <iostream>
#include <vector>
#include <array>
#include <string>

#include "Core\OpenGL Classes\GLDebug\GLDebug.h"
#include "Core\Dependencies\imgui\imgui.h"
#include "Core\Engine\Collision\AABB.h"
#include "Core\Engine\Background\Background.h"
#include "Core\Engine\Instance\Instance.h"
#include "Core\Engine\Scene\SceneData.h"
#include "Core\OpenGL Classes\Index Buffer\IndexBuffer.h"
#include "Core\OpenGL Classes\Vertex Array\VertexArray.h"
#include "Core\OpenGL Classes\Vertex Buffer\VertexBuffer.h"
#include "Core\OpenGL Classes\Texture\Texture.h"
#include "Core\OpenGL Classes\Shader\Shader.h"
#include "Core\Engine\Logger\Log.h"

namespace GLGame
{
	struct GenericObject
	{
		GLfloat coords[12];
		GLfloat tex_coords[8];
	};

	// Reliable batch renderer. Can render upto 10000 quads

	class SpriteBatcher
	{
	public : 

		SpriteBatcher();
		~SpriteBatcher();
		void StartSpriteBatch(Camera* scene_camera);
		void StartSpriteBatch(const glm::mat4& view_projection_matrix);
		void StartSpriteBatch();
		void EndSpriteBatch();
		void AddGenericTextureToBatch(Texture* texture, const glm::vec3 &pos);
		void AddSpriteToBatch(SceneDataItem item);

	private : 

		void DrawFullBatch();

		glm::mat4 m_ViewProjectionMatrix;
		AABB m_CameraCull;

		int m_MaximumTextureSlots;
		const int m_MaximumQuads;
		int m_LastElementTex;
		int m_LastElementVBuff;
		unsigned int m_VerticesWritten;
		int m_SamplerArray[32];
		GLfloat *m_VertexBuffer;
		GLuint *m_IndexBuffer;
		int m_SlottedTextures[32];
		int m_CurrSlottedTexElement = 0;
		bool m_CameraCullGiven = false;

		// OpenGL Specific objects
		VertexBuffer m_VBO;
		VertexArray m_VAO;
		IndexBuffer m_IBO;
		Shader m_Shader;

		glm::vec3 m_CameraPos;
	};

	// Quad Batch Renderer

	class QuadBatcher
	{
	public:

		QuadBatcher();
		~QuadBatcher();
		void StartQuadBatch(glm::mat4& view_projection);
		void AddQuadToBatch(const glm::vec4& position, const glm::vec4 color, const glm::mat4 model_matrix = glm::mat4(1.0f));
		void EndQuadBatch();

	private:

		void DrawFullBatch();

		glm::mat4 m_ViewProjectionMatrix;

		const int m_MaximumQuads;
		int m_LastElementTex;
		int m_LastElementVBuff;
		unsigned int m_VerticesWritten;
		
		GLfloat* m_VertexBuffer;
		GLuint* m_IndexBuffer;

		// OpenGL Specific objects
		VertexBuffer m_VBO;
		VertexArray m_VAO;
		IndexBuffer m_IBO;
		Shader m_Shader;

		Camera* m_Camera;
	};

	// This is new type of batch rendering that I wanted to experiment with
	void BatchRenderGenericObjects(vector <GenericObject> &objects, unsigned int texture_id, Shader shader, glm::mat4 model);
	void BatchRenderObjectInstances(vector <SceneDataItem>& objects, Shader shader, const glm::mat4& model_matrix, const glm::mat4& view_matrix, const glm::mat4& view_projection_matrix, const glm::vec3& camera_scale, AABB camera_view_cull);
	void BatchRenderSpriteInstances(vector <SceneDataItem>& sprites);
}