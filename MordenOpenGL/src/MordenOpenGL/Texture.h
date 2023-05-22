#pragma once
#include "glad/glad.h"

class Texture
{
public:
	Texture(const std::string& filePath);
	~Texture();

	void Bind(uint32_t slot);

	void SetData(void* data, uint32_t size);

private:
	uint32_t m_RendererID;
	std::string m_FilePath;

	uint32_t m_Width;
	uint32_t m_Height;

	GLenum m_InternalFormat, m_DataFormat;

};

//// 纹理1
//uint32_t texture1;
//glGenTextures(1, &texture1);
//glBindTexture(GL_TEXTURE_2D, texture1);

//// 为当前绑定纹理设置环绕，过滤方式
//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//
//// 加载并生成纹理
//int width, height, nrChannels;
//// OpenGL要求y轴0.0坐标是在图片的底部的，但是图片的y轴0.0坐标通常在顶部。很幸运，stb_image.h能够在图像加载时帮助我们翻转y轴
//stbi_set_flip_vertically_on_load(true);
//unsigned char* data = stbi_load("asserts/textures/R-C.jpg", &width, &height, &nrChannels, 0);
//if (data)
//{
//	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
//	glGenerateMipmap(GL_TEXTURE_2D);
//}
//else
//{
//	std::cout << "纹理加载错误" << std::endl;
//}
//
//stbi_image_free(data);