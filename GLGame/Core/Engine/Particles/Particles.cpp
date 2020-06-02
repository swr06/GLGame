#include "Particles.h"

namespace GLGame
{
	ParticleSystem::ParticleSystem() : m_QuadVB(GL_ARRAY_BUFFER)
	{
		m_ParticlePool.resize(1000);

		float default_vertices[] = {
				 -0.5f, -0.5f, 0.0f,
				  0.5f, -0.5f, 0.0f,
				  0.5f,  0.5f, 0.0f,
				 -0.5f,  0.5f, 0.0f
		};

		uint32_t elements[] = 
		{
			0, 1, 2, 2, 3, 0
		};

		m_QuadVA.Bind();
		m_QuadVB.BufferData(sizeof(default_vertices), default_vertices, GL_STATIC_DRAW);
		m_QuadVB.VertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), 0);
		m_QuadIB.BufferData(sizeof(elements), elements, GL_STATIC_DRAW);
		m_QuadVA.Unbind();

		m_ParticleShader.CreateShaderProgram(GLGAME_DEFAULT_PARTICLE_VERTEX, GLGAME_DEFAULT_PARTICLE_FRAGMENT);;
	}

	void ParticleSystem::OnUpdate(double ts)
	{
		for (auto& particle : m_ParticlePool)
		{
			if (!particle.Active)
				continue;

			if (particle.LifeRemaining <= 0.0f)
			{
				particle.Active = false;
				continue;
			}

			particle.LifeRemaining -= ts;
			particle.Position += particle.Velocity * (float)ts;
			particle.Rotation += 0.01f * ts;
		}
	}

	void ParticleSystem::OnRender(Camera& camera)
	{
		m_ParticleShader.Use();
		m_ParticleShader.SetMatrix4("u_ViewProj", camera.GetProjectionMatrix());

		for (auto& particle : m_ParticlePool)
		{
			if (!particle.Active)
				continue;

			float life = particle.LifeRemaining / particle.LifeTime;
			glm::vec4 color = glm::lerp(particle.ColorEnd, particle.ColorBegin, life);

			float size = glm::lerp(particle.SizeEnd, particle.SizeBegin, life);

			glm::mat4 transform = glm::translate(glm::mat4(1.0f), { particle.Position.x, particle.Position.y, 0.0f })
				* glm::rotate(glm::mat4(1.0f), particle.Rotation, { 0.0f, 0.0f, 1.0f })
				* glm::scale(glm::mat4(1.0f), { size, size, 1.0f });


			m_ParticleShader.SetMatrix4("u_Transform", transform);
			m_ParticleShader.SetVector4f("u_Color", color);

			DebugGLFunction(m_QuadVA.Bind());
			glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, (void*) 0);
			m_QuadVA.Unbind();
		}
	}

	void ParticleSystem::Emit(const ParticleProps& particleProps)
	{
		Particle& particle = m_ParticlePool[m_PoolIndex];
		particle.Active = true;
		particle.Position = particleProps.Position;
		particle.Rotation = Random::Float() * 2.0f * glm::pi<float>();

		// Velocity
		particle.Velocity = particleProps.Velocity;
		particle.Velocity.x += particleProps.VelocityVariation.x * (Random::Float() - 0.5f);
		particle.Velocity.y += particleProps.VelocityVariation.y * (Random::Float() - 0.5f);

		// Color
		particle.ColorBegin = particleProps.ColorBegin;
		particle.ColorEnd = particleProps.ColorEnd;

		particle.LifeTime = particleProps.LifeTime;
		particle.LifeRemaining = particleProps.LifeTime;
		particle.SizeBegin = particleProps.SizeBegin + particleProps.SizeVariation * (Random::Float() - 0.5f);
		particle.SizeEnd = particleProps.SizeEnd;

		m_PoolIndex = --m_PoolIndex % m_ParticlePool.size();
	}

}