#include "Shader.h"

Shader::Shader(const std::string& vertexPath, const std::string& fragmentPath) {
	std::string vertexSource = ReadShader(vertexPath);
	std::string fragmentSource = ReadShader(fragmentPath);

	unsigned int vertex = CompileShader(GL_VERTEX_SHADER, vertexSource.c_str());
	unsigned int fragment = CompileShader(GL_FRAGMENT_SHADER, fragmentSource.c_str());

	int success = 0;
	char infoLog[512];

	m_ShaderProgram = glCreateProgram();
	glAttachShader(m_ShaderProgram, vertex);
	glAttachShader(m_ShaderProgram, fragment);
	glLinkProgram(m_ShaderProgram);

	glGetProgramiv(m_ShaderProgram, GL_LINK_STATUS, &success);
	if (!success)
	{
		glGetProgramInfoLog(m_ShaderProgram, 512, NULL, infoLog);
		std::cout << "ERROR! Shader program not linked correctly \n" << infoLog << std::endl;
	}

	glDeleteShader(vertex);
	glDeleteShader(fragment);
}

Shader::~Shader() {
	//holder
}

void Shader::Bind() {
	glUseProgram(m_ShaderProgram);
}

void Shader::UnBind() {
	glUseProgram(0);
}

void Shader::SetInt(const std::string& name, const int value) {
	glUniform1i(GetUniformLocation(name), value);
}

void Shader::SetFloat(const std::string& name, const float value) {
	glUniform1f(GetUniformLocation(name), value);
}

void Shader::SetMat4fv(const std::string& name, const glm::mat4 matrix) {
	glUniformMatrix4fv(GetUniformLocation(name), 1, GL_FALSE, glm::value_ptr(matrix));
}

int Shader::GetUniformLocation(const std::string &name) {
	if (m_LocationCache.find(name) != m_LocationCache.end()) {
		return m_LocationCache[name];
	}

	int location = glGetUniformLocation(m_ShaderProgram, name.c_str());

	if (location == -1) {
		std::cout << "Uniform " << name << " not found" << std::endl;
		return location;
	}

	m_LocationCache[name] = location;
	return location;
}

std::string Shader::ReadShader(const std::string& filePath) {
	std::ifstream reader(filePath);
	std::stringstream stream;

	if (!reader.is_open()) {
		std::cout << "Unable to read file" << std::endl;
		return "";
	}

	stream << reader.rdbuf();
	reader.close();

	return stream.str();
}

unsigned int Shader::CompileShader(unsigned int type, const char * source) {
	unsigned int shaderType = glCreateShader(type);
	glShaderSource(shaderType, 1, &source, nullptr);
	glCompileShader(shaderType);

	int success = 0;
	char infoLog[512];

	glGetShaderiv(shaderType, GL_COMPILE_STATUS, &success);

	if (!success) {
		glGetShaderInfoLog(shaderType, 512, NULL, infoLog);
		std::string stringType = (type == GL_VERTEX_SHADER) ? "Vertex" : "Fragment";

		std::cout << stringType << " shader failed to compile \n" << infoLog << std::endl;
	}

	return shaderType;
}
