#include "Object.h"

namespace GLGame
{
	// This function is deprecated.An std::string is not used as the ID
	/*uint32_t GenerateObjectID()
	{
		return (int) abs(rand() % INT_MAX);
	}*/
	 
	Object::Object(const string& id, bool should_cull, bool visible, Shader* shader, bool record_collision_event, const glm::vec4& bounding_box) : m_Sprite(nullptr), m_Rotation(0.0f)
	{
		auto start = chrono::steady_clock::now();
		const char* obj_append = "@#$*#\0";

		m_Shader = shader;
		m_ID = id;
		m_ID.insert(0, obj_append);
		m_DisplayName = id;
		m_HasSprite = false;
		m_RecordCollision = record_collision_event;
		m_BoundingBox = bounding_box;
		m_Cull = should_cull;
		m_Visible = visible;
		GameInternal::_IntRegisterObject(this);

		auto end = chrono::steady_clock::now();
		double elapsed_time = std::chrono::duration_cast<std::chrono::duration<double>>(end - start).count();
	
		// a small glitch that occurs

		if (elapsed_time < 0)
		{
			elapsed_time = 0;
		}

		Log::_LogObjectCreation(glfwGetTime(),elapsed_time, m_DisplayName, visible, should_cull, true, __FILE__, __LINE__);
	}

	Object::Object(const string& id, Sprite& sprite, bool should_cull, bool visible, Shader* shader, bool record_collision_event, const glm::vec4& bounding_box) : m_Rotation(-45.0f)
	{
		auto start = chrono::steady_clock::now();
		const char* obj_append = "@#$*#\0";

		m_ID = id;
		m_ID.insert(0, obj_append);
		m_DisplayName = id;
		m_Shader = shader;
		m_HasSprite = true;
		m_Sprite = &sprite;
		m_RecordCollision = record_collision_event;
		m_Cull = should_cull;
		m_Visible = visible;

		GameInternal::_IntRegisterObject(this);

		if (bounding_box.x == -1.0f && bounding_box.y == -1.0f &&
			bounding_box.z == -1.0f && bounding_box.w == -1.0f)
		{
			m_BoundingBox = { 0.0f, 0.0f, sprite.GetCurrentTexture()->GetWidth(), sprite.GetCurrentTexture()->GetWidth() };
		}
		
		else
		{
			m_BoundingBox = bounding_box;
		}

		m_ModelMatrix = glm::mat4(1.0f);

		GameInternal::_IntRegisterObject(this);
		auto end = chrono::steady_clock::now();

		double elapsed_time = std::chrono::duration_cast<std::chrono::duration<double>>(end - start).count();

		// a small glitch that occurs

		if (elapsed_time < 0)
		{
			elapsed_time = 0;
		}

		Log::_LogObjectCreation(glfwGetTime(), elapsed_time, m_DisplayName, visible, should_cull, true, __FILE__, __LINE__);
	}

	void Object::ApplyRotationTransformation(float angle)
	{
		m_ModelMatrix = glm::rotate(glm::mat4(m_ModelMatrix), glm::radians(angle), glm::vec3(0,0,1));
	}

	void Object::ApplyScaleTransformation(const glm::vec3 &scale_factor)
	{
		m_ModelMatrix = glm::scale(glm::mat4(m_ModelMatrix), glm::vec3(scale_factor));
	}

	void Object::SetShader(Shader& shader)
	{
		m_Shader = &shader; 
	}

	void Object::SetSprite(Sprite& sprite)
	{
		if (m_BoundingBox == glm::vec4(0.0f, 0.0f, 0.0f, 0.0f))
		{
			m_BoundingBox.x = 0.0f;
			m_BoundingBox.y = 0.0f;
			m_BoundingBox.z = sprite.GetCurrentTextureWidth();
			m_BoundingBox.w = sprite.GetCurrentTextureHeight();
		}

		m_HasSprite = true;
		m_Sprite = &sprite;
	}

	void Object::SetModelMatrix(const glm::mat4& model)
	{
		m_ModelMatrix = model; 
	}

	void Object::SetVisibility(bool visible)
	{
		m_Visible = visible; 
	}

	void Object::SetPosition(const glm::vec3& pos)
	{
		GameInternal::_UpdateObjectPosition(this->m_ID, pos);
	}

	void Object::IncrementPosition(const glm::vec3& increment)
	{
		GameInternal::_IncrementObjectPosition(this->m_ID, increment);
	}

	void Object::SetBoundingBox(const glm::vec4& bounding_box)
	{
		m_MaskType = mask_rect;
		m_BoundingBox = bounding_box;
	}

	void Object::RemoveSprite()
	{
		m_Sprite = 0;
		m_HasSprite = false; 
	}

	glm::vec3 Object::GetPosition(int instance_id, glm::vec3& coords)
	{
		return glm::vec3();
	}

	const glm::mat4& Object::GetModelMatrix()
	{
		return m_ModelMatrix;
	}

	void Object::IntUpdate(long long fps)
	{
		if (m_HasSprite)
		{
			this->m_Sprite->UpdateSprite(fps); 
		}
	}
}