#include "hzpch.h"

#include "Shader.h"

#include <cassert>
#include <fstream>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

static GLenum ShaderTypeFromString(const std::string& type)
{
	if (type == "vertex") return GL_VERTEX_SHADER;
	if (type == "fragment" || type == "pixel") return GL_FRAGMENT_SHADER;

	// static_assert(false, "Unkown shader type");
	return 0;
}

Shader::Shader(const std::string& filePath):m_FilePath(filePath)
{
	std::string source = ReadFile(filePath);

	auto shaderSource = PreProcess(source);

	CompileShader(shaderSource);
}

uint32_t Shader::GetRendererID()
{
	return m_RendererID;
}

void Shader::SetFloat(const std::string& name, float value)
{
	glUniform1f(glGetUniformLocation(m_RendererID, name.c_str()), value);
}

void Shader::SetInt(const std::string& name, int value)
{
	glUniform1i(glGetUniformLocation(m_RendererID, name.c_str()), value);
}

void Shader::SetIntArray(const std::string& name, int* values, uint32_t count)
{
	glUniform1iv(glGetUniformLocation(m_RendererID, name.c_str()), count, values);
}

void Shader::SetFloat3(const std::string& name, const glm::vec3& value)
{
	glUniform3f(glGetUniformLocation(m_RendererID, name.c_str()), value.x, value.y, value.z);
}

void Shader::SetFloat4(const std::string& name, const glm::vec4& value)
{
	glUniform4f(glGetUniformLocation(m_RendererID, name.c_str()), value.x, value.y, value.z, value.w);
}

void Shader::SetMat4(const std::string& name, const glm::mat4& value)
{
	glUniformMatrix4fv(glGetUniformLocation(m_RendererID, name.c_str()), 1, GL_FALSE, glm::value_ptr(value));

}

std::string Shader::ReadFile(const std::string& filepath)
{
	std::string result;
	std::ifstream in(filepath, std::ios::in, std::ios::binary);

	if (in)
	{
		in.seekg(0, std::ios::end);
		result.resize(in.tellg());
		in.seekg(0, std::ios::beg);
		in.read(&result[0], result.size());
		in.close();
	}
	else
	{
		std::cout << R"(Could not open file ')" << filepath << R"(';)" << std::endl;
	}

	return result;
}

std::unordered_map<GLenum, std::string> Shader::PreProcess(const std::string& source)
{
	std::unordered_map<GLenum, std::string> shaderSources;

	const char* typeToken = "#type";

	size_t typeTokenLength = strlen(typeToken);
	size_t pos = source.find(typeToken, 0);
	while (pos != std::string::npos)
	{
		size_t eol = source.find_first_of("\r\n", pos);
		assert(eol != std::string::npos, "Syntax error");
		size_t begin = pos + typeTokenLength + 1;
		std::string type = source.substr(begin, eol - begin);
		assert(ShaderTypeFromString(type), "Invalid shader type specific");

		size_t nextLinePos = source.find_first_not_of("\r\n", eol);
		assert(nextLinePos != std::string::npos, "Syntax error");
		pos = source.find(typeToken, nextLinePos);
		shaderSources[ShaderTypeFromString(type)] = source.substr(nextLinePos, pos - (nextLinePos == std::string::npos ? source.size() - 1 : nextLinePos));
	}

	return shaderSources;
}

void Shader::CompileShader(const std::unordered_map<GLenum, std::string>& shaderSource)
{
	std::vector<uint32_t>shaderVec;
	for (auto &&[type, source] : shaderSource)
	{
		uint32_t shader;
		shader = glCreateShader(type);
		const char* source_c_str = source.c_str();
		glShaderSource(shader, 1, &source_c_str, nullptr);
		glCompileShader(shader);

		int success;
		char infoLog[512];
		glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
		if (!success)
		{
			glGetShaderInfoLog(shader, 512, nullptr, infoLog);
			std::cout << "ERROR::SHADER::VERTEX::COMPILATION_FAILED\n" << infoLog << std::endl;
			assert(false);
		}

		shaderVec.push_back(shader);
	}

	GLuint program = glCreateProgram();
	for (auto shader : shaderVec)
	{
		glAttachShader(program, shader);
	}
	glLinkProgram(program);

	int success;
	char infoLog[512];

	// 检查链接结果
	glGetProgramiv(program, GL_LINK_STATUS, &success);
	if (!success) {
		glGetProgramInfoLog(program, 512, nullptr, infoLog);
		std::cout << "ERROR::SHADER::PROGRAM::LINKING_FAILED\n" << infoLog << std::endl;
		assert(false);
	}

	// 删除着色器
	for (auto shader : shaderVec)
	{
		glDeleteShader(shader);
	}
	shaderVec.clear();

	m_RendererID = program;
}

