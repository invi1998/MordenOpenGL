#pragma once
#include "glad/glad.h"

enum TEXTURE_TYPE
{
	DIFFUSE,		// ��������ͼ
	SPECULAR,		// ����߹���ͼ
	NORMAL,			// ������ͼ
	HEIGHT			// �����ͼ
};

class Texture
{
public:
	Texture(const std::string& filePath, TEXTURE_TYPE type);
	~Texture();

	void Bind(uint32_t slot);

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

};

//// ����1
//uint32_t texture1;
//glGenTextures(1, &texture1);
//glBindTexture(GL_TEXTURE_2D, texture1);

//// Ϊ��ǰ���������û��ƣ����˷�ʽ
//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
//glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
//
//// ���ز���������
//int width, height, nrChannels;
//// OpenGLҪ��y��0.0��������ͼƬ�ĵײ��ģ�����ͼƬ��y��0.0����ͨ���ڶ����������ˣ�stb_image.h�ܹ���ͼ�����ʱ�������Ƿ�תy��
//stbi_set_flip_vertically_on_load(true);
//unsigned char* data = stbi_load("asserts/textures/R-C.jpg", &width, &height, &nrChannels, 0);
//if (data)
//{
//	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);
//	glGenerateMipmap(GL_TEXTURE_2D);
//}
//else
//{
//	std::cout << "������ش���" << std::endl;
//}
//
//stbi_image_free(data);