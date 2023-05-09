#pragma once

#include <glad/glad.h>

class Shader
{
public:
	Shader(const std::string& filePath);

	uint32_t GetRendererID();

private:
	std::string ReadFile(const std::string& filepath);
	std::unordered_map<GLenum, std::string> PreProcess(const std::string& source);
	void CompileShader(const std::unordered_map<GLenum, std::string>& shaderSource);

private:
	uint32_t m_RendererID;
	std::string m_Name;
	std::string m_FilePath;

};
