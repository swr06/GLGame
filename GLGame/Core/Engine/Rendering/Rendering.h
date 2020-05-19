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
#include "Core\Engine\Background\Background.h"
#include "Core\Engine\Scene\SceneData.h"
#include "Core\OpenGL Classes\Index Buffer\IndexBuffer.h"
#include "Core\OpenGL Classes\Vertex Array\VertexArray.h"
#include "Core\OpenGL Classes\Vertex Buffer\VertexBuffer.h"
#include "Core\OpenGL Classes\Texture\Texture.h"
#include "Core\OpenGL Classes\Shader\Shader.h"
#include "Core\Engine\Logger\Log.h"

namespace GLGame
{
	struct _RenderItem
	{
		float x;
		float y;
		float z;
		float tw;
		float th;
		Texture* tex;
	};

	void NormallyRenderBackgrounds(SceneBackground* bg, Shader shader, float w, float h, const glm::mat4& model_matrix, const glm::mat4& view_matrix, const glm::mat4& view_projection_matrix);
	void _NormallyRenderSEItems(_RenderItem item, Shader shader, const glm::mat4& projection_matrix);
}
