#include "hzpch.h"
#include "Texture.h"

#include "stb_image.h"
#include "glad/glad.h"

Texture::Texture(const std::string& filePath, TEXTURE_TYPE type):m_FilePath(filePath), m_Type(type)
{
	int width, height, channels;
	stbi_set_flip_vertically_on_load(1);
	stbi_uc* data = nullptr;

	data = stbi_load(m_FilePath.c_str(), &width, &height, &channels, 0);

	if (data)
	{
		m_Width = width;
		m_Height = height;

		GLenum internalFormat = 0, dataFormat = 0;
		if (channels == 4)
		{
			internalFormat = GL_RGBA8;
			dataFormat = GL_RGBA;
		}
		else if (channels == 3)
		{
			internalFormat = GL_RGB8;
			dataFormat = GL_RGB;
		}
		else if (channels == 1)
		{
			internalFormat = GL_RED;
			dataFormat = GL_RED;
		}

		m_InternalFormat = internalFormat;
		m_DataFormat = dataFormat;

		if (!(internalFormat & dataFormat))
		{
			std::cout << "Format not supported!" << std::endl;
		}

		// glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);
		// glTextureStorage2D(m_RendererID, 1, internalFormat, m_Width, m_Height);

		// glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
		// glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		// glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);
		// glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);

		// glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, dataFormat, GL_UNSIGNED_BYTE, data);

		glGenTextures(1, &m_RendererID);
		glBindTexture(GL_TEXTURE_2D, m_RendererID);
		glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, dataFormat, GL_UNSIGNED_BYTE, data);
		glGenerateMipmap(GL_TEXTURE_2D);

		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

		stbi_image_free(data);
	}
}

Texture::~Texture()
{
	glDeleteTextures(1, &m_RendererID);
}

void Texture::Bind(uint32_t slot) const
{
	// glUseProgram(m_RendererID);
	glBindTexture(slot, m_RendererID);
	// glBindTextureUnit(slot, m_RendererID);
}

void Texture::SetData(void* data, uint32_t size)
{
	uint32_t bpp = m_DataFormat == GL_RGBA ? 4 : 3;

	if (size != m_Width * m_Height * bpp)
	{
		std::cout << "Data must be entire texture!" << std::endl;
		return;
	}

	glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, m_DataFormat, GL_UNSIGNED_BYTE, data);
}

