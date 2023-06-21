#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>

class Shader
{
public:
	Shader(const std::string& filePath);
	~Shader();

	uint32_t GetRendererID();

	void Use();

	void SetBool(const std::string& name, bool value);
	void SetFloat(const std::string& name, float value);
	void SetInt(const std::string& name, int value);
	void SetIntArray(const std::string& name, int* values, uint32_t count);
	void SetFloat3(const std::string& name, const glm::vec3& value);
	void SetFloat4(const std::string& name, const glm::vec4& value);
	void SetMat4(const std::string& name, const glm::mat4& value);
	void SetVec2(const std::string& name, const glm::vec2& value);
	void SetVec3(const std::string& name, const glm::vec3& value);
	void SetVec4(const std::string& name, const glm::vec4& value);

private:
	std::string ReadFile(const std::string& filepath);
	std::unordered_map<GLenum, std::string> PreProcess(const std::string& source);
	void CompileShader(const std::unordered_map<GLenum, std::string>& shaderSource);


private:
	uint32_t m_RendererID;
	std::string m_Name;
	std::string m_FilePath;

};
