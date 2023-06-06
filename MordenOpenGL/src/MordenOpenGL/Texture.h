#pragma once
#include "glad/glad.h"

enum TEXTURE_TYPE
{
	DIFFUSE,		// 漫反射贴图
	SPECULAR,		// 镜面高光贴图
	NORMAL,			// 法线贴图
	HEIGHT			// 深度贴图
};

class Texture
{
public:
	Texture(const std::string& filePath, TEXTURE_TYPE type);
	Texture(const std::initializer_list<std::string>& filePaths);
	~Texture();

	void Bind(uint32_t slot) const;

	void SetData(void* data, uint32_t size);

	uint32_t GetRendererID() const { return m_RendererID; }

	inline const TEXTURE_TYPE GetType() const { return m_Type; }

	std::string GetFilePath() const { return m_FilePath; }

private:
	TEXTURE_TYPE m_Type;
	uint32_t m_RendererID;
	std::string m_FilePath;

	uint32_t m_Width;
	uint32_t m_Height;

	GLenum m_InternalFormat, m_DataFormat;

	std::vector<std::string> m_CubeFacePath;

};

// 从6个单独的纹理面加载立方体纹理贴图
// 顺序:
// +X (right)
// -X (left)
// +Y (top)
// -Y (bottom)
// +Z (front) 
// -Z (back)
