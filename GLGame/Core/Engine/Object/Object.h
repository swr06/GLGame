#pragma once

#include <GL/glew.h>
#include <GLFW\glfw3.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Core\OpenGL Classes\GLDebug\GLDebug.h"

#include <stdio.h>
#include <stdlib.h>

#include "Core\Engine\Sprite\Sprite.h"
#include "Core\OpenGL Classes\Shader\Shader.h"
#include "Core\Engine\Macros\Macros.h"

namespace GLGame
{
	//static uint32_t GenerateObjectID(); // This function is deprecated. An std::string is not used as the ID

	class Object;

	namespace GameInternal
	{
		void _IntRegisterObject(Object* object);
		void _UpdateObjectPosition(const string& id, const glm::vec3& pos);
		void _GetObjectPosition(const string& id, uint32_t instance_id, int layer, glm::vec3& pos);
		void _IncrementObjectPosition(const string& id, const glm::vec3& amt);
		void _IntDeregisterObject(Object* object);
	}

	enum ObjectTransformations
	{
		rotate = 0,
		scale
	};

	enum ObjectCollisionMaskType
	{
		mask_rect = 0,
		mask_circle
	};

	class Object
	{
	public:

		Object(const string& id, bool should_cull = true, bool visible = true, Shader* shader = nullptr, bool record_collision_event = false, const glm::vec4& bounding_box = { 0.0f,0.0f,0.0f,0.0f });
		Object(const string& id, Sprite& sprite, bool should_cull = true, bool visible = true, Shader* shader = nullptr, bool record_collision_event = false, const glm::vec4& bounding_box = { -1.0f,-1.0f,-1.0f,-1.0f });
		~Object();

		// Applies a rotation to the object
		void ApplyRotationTransformation(float angle);

		// Applies a scale to the object
		void ApplyScaleTransformation(const glm::vec3& scale_factor);

		// Sets a shader for the object
		void SetShader(Shader& shader);

		// Sets a sprite for the object
		void SetSprite(Sprite& sprite);

		// Sets a predefined matrix to the model matrix value of the object
		// Model matrices are used for transformations
		void SetModelMatrix(const glm::mat4& model);

		// Whether or not the object should be visible. Note that the current texture will be 
		// updated if it's sprite is an animation
		void SetVisibility(bool visible);

		// Sets the position of the object in the current scene
		void SetPosition(const glm::vec3& pos);

		// Increments the position of the object in the current scene
		void IncrementPosition(const glm::vec3& increment);

		// Sets a bounding box which is used for collision detection.
		void SetBoundingBox(const glm::vec4& bounding_box);

		void SetBoundingCircle(const float radius, const float position)
		{
			Log::LogToConsole("Circle collisions have to still be implemented...");
		}

		const glm::vec4& GetBoundingBox() const { return m_BoundingBox; }

		// Removes the sprite of the object. 
		void RemoveSprite(); 

		// Don't use this function externally unless you want to reference it from the global array
		// It has some extra characters inserted to prevent duplication.
		const string& GetObjectID() const { return m_ID; };

		// This is the function to use when you want to get the name of your object
		// It returns the name that is specified during construction
		const string& GetObjectDisplayName() const { return m_DisplayName; }

		// Returns the position of the (n)th instance in the scene
		glm::vec3 GetPosition(int instance_id, glm::vec3& coords);

		// Returns the sprite of the object
		Sprite* GetSprite () const { return m_Sprite; }

		// Returns the shader of the object. If it hasn't been set, It may return nothing
		Shader* GetShader() const
		{ 
			if (m_Shader != nullptr)
			{
				return m_Shader;
			}

			else
			{
				return nullptr;
			}
		}

		// Returns true if the object has a shader. Otherwise, it returns false
		bool HasShader() const 
		{
			if (m_Shader == nullptr)
			{
				return false;
			}

			else
			{
				return true;
			}
		}

		// Returns true if the object is visible. Otherwise, it returns false
		bool IsVisible() const { return m_Visible; }

		// Returns true if the object should be culled. Otherwise, it returns false
		bool Cull() const { return m_Cull; }

		// Returns true if the object has a sprite. Otherwise, it returns false
		bool HasSprite() const { return m_HasSprite; }

		// Returns the display name. This display name is also used in the scene editor
		// Useful for debugging 
		const string& GetDisplayName() const {	return m_DisplayName;   }

		// Returns the model matrix. Which when multiplied in the vertex shader, can result in some transformations
		const glm::mat4& GetModelMatrix();

		// Updates the object if it has a sprite and that sprite has an animation attached
		void IntUpdate(long long fps);

		// Returns the type of collision mask set. 
		// can either be mask_rect or mask_circle
		const ObjectCollisionMaskType& GetCollisionMaskType() const { return m_MaskType; }

	private:

		bool m_HasSprite;
		bool m_Cull;
		bool m_RecordCollision;
		float m_Rotation;

		Sprite* m_Sprite;
		Shader* m_Shader;
		glm::mat4 m_ModelMatrix;
		glm::vec4 m_BoundingBox;

		bool m_Visible;
		string m_ID;
		string m_DisplayName;

		ObjectCollisionMaskType m_MaskType = mask_rect;
	};
}