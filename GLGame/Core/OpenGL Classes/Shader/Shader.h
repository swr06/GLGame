#pragma once

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Core\OpenGL Classes\GLDebug\GLDebug.h"

#include "Core\Engine\Logger\Log.h"

#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <fstream>
#include <chrono>
#include <unordered_map>
#include <filesystem>

namespace GLGame
{
	using namespace std;
	class Shader
	{
	public:

		Shader(string vertex_path, const string fragment_path)
		{
			CreateShaderProgram(vertex_path, fragment_path);
		}

		Shader() {}; 


		~Shader();

		GLuint CreateShaderProgram(const string vertex_pth, const string fragment_pth);
		GLuint GetProgramID();
		inline void Use()
		{
			glUseProgram(this->Program);
		}

		void SetFloat(const GLchar* name, GLfloat value, GLboolean useShader = GL_FALSE);
		void SetInteger(const GLchar* name, GLint value, GLboolean useShader = GL_FALSE);
		void SetVector2f(const GLchar* name, GLfloat x, GLfloat y, GLboolean useShader = GL_FALSE);
		void SetVector2f(const GLchar* name, const glm::vec2& value, GLboolean useShader = GL_FALSE);
		void SetVector3f(const GLchar* name, GLfloat x, GLfloat y, GLfloat z, GLboolean useShader = GL_FALSE);
		void SetVector3f(const GLchar* name, const glm::vec3& value, GLboolean useShader = GL_FALSE);
		void SetVector4f(const GLchar* name, GLfloat x, GLfloat y, GLfloat z, GLfloat w, GLboolean useShader = GL_FALSE);
		void SetVector4f(const GLchar* name, const glm::vec4& value, GLboolean useShader = GL_FALSE);
		void SetMatrix4(const GLchar* name, const glm::mat4& matrix, GLboolean useShader = GL_FALSE);
		void SetIntegerArray(const GLchar* name, const GLint* value, GLsizei count, GLboolean useShader = GL_FALSE);

	 private:

		unordered_map<string, GLint> Location_map; // To avoid unnecessary calls to glGetUniformLocation()
		GLuint Program = 0;

		GLint GetUniformLocation(string uniform_name);
	};
}
