#pragma once
#include <iostream>
#include <string>
#include <sstream>
#include <fstream>
#include <unordered_map>
#include <GL/glew.h>
#include "Vendor/glm/glm.hpp"
#include "Vendor/glm/gtc/matrix_transform.hpp"
#include "Vendor/glm/gtc/type_ptr.hpp"

class Shader
{
public:
	Shader(const std::string& vertexPath, const std::string& fragmentPath);
	~Shader();

	void Bind();
	void UnBind();

	void SetInt(const std::string& name, const int value);
	void SetFloat(const std::string& name, const float value);
	void SetMat4fv(const std::string& name, const glm::mat4 matrix);

private:
	std::string ReadShader(const std::string& shaderFile);
	unsigned int CompileShader(unsigned int type, const char * source);

	int GetUniformLocation(const std::string& name);

	unsigned int m_ShaderProgram;
	std::unordered_map<std::string, int> m_LocationCache;
};

