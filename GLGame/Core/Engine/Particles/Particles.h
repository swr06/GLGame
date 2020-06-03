#pragma once

/*
	This particle system is highly inspired by Yan Chernikov's particle system.
	I improvised it by adding :
		- Batch Rendering
		- Texturing
		- Other optimizations

	- Samuel Rasquinha
*/

#include <iostream>
#include <string>
#include <vector>

#include <GL/glew.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include <glm/gtc/constants.hpp>
#include <glm/gtx/compatibility.hpp>

#include "Core\OpenGL Classes\Random\Random.h"
#include "Core\OpenGL Classes\GLDebug\GLDebug.h"
#include "Core\Engine\Macros\Macros.h"
#include "Core\Engine\Logger\Log.h"
#include "Core\OpenGL Classes\Index Buffer\IndexBuffer.h"
#include "Core\OpenGL Classes\Vertex Array\VertexArray.h"
#include "Core\OpenGL Classes\Vertex Buffer\VertexBuffer.h"
#include "Core\OpenGL Classes\Texture\Texture.h"
#include "Core\OpenGL Classes\Shader\Shader.h"
#include "Core\Engine\Camera\Camera.h"
#include "ParticleBatcher.h"

namespace GLGame
{
	struct ParticleProps
	{
		glm::vec2 Position;
		glm::vec2 Velocity, VelocityVariation;
		glm::vec4 ColorBegin, ColorEnd;
		float SizeBegin, SizeEnd, SizeVariation;
		float LifeTime = 1.0f;
	};

	class ParticleSystem
	{
	public:
		ParticleSystem();

		void OnUpdate(double ts);
		void OnRender(Camera& camera);

		void Emit(const ParticleProps& particleProps);

	private:

		struct Particle
		{
			glm::vec2 Position;
			glm::vec2 Velocity;
			glm::vec4 ColorBegin, ColorEnd;
			float Rotation = 0.0f;
			float SizeBegin, SizeEnd;

			float LifeTime = 1.0f;
			float LifeRemaining = 0.0f;

			bool Active = false;
		};

		std::vector<Particle> m_ParticlePool;
		uint32_t m_PoolIndex = 999;

		VertexArray m_QuadVA;
		VertexBuffer m_QuadVB;
		IndexBuffer m_QuadIB;
		Shader m_ParticleShader;
		GLint m_ParticleShaderViewProj, m_ParticleShaderTransform, m_ParticleShaderColor;
	};
}