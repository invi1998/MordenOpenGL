#pragma once

#include <glad/glad.h>

class Shader
{
public:
	Shader(const std::string& filePath);

	uint32_t GetRendererID();


	void SetFloat(const std::string& name, float value);
	void SetFloat3(const std::string& name, const glm::vec3& value);
	void SetFloat4(const std::string& name, const glm::vec4& value);
	void SetMat4(const std::string& name, const glm::mat4& value);
	void SetInt(const std::string& name, int value);
	void SetIntArray(const std::string& name, int* values, uint32_t count);

private:
	std::string ReadFile(const std::string& filepath);
	std::unordered_map<GLenum, std::string> PreProcess(const std::string& source);
	void CompileShader(const std::unordered_map<GLenum, std::string>& shaderSource);


private:
	uint32_t m_RendererID;
	std::string m_Name;
	std::string m_FilePath;

};
