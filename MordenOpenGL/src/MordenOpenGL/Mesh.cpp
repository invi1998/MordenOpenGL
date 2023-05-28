#include "hzpch.h"
#include "Mesh.h"

Mesh::Mesh(std::vector<Vertex>& vertics, std::vector<uint32_t> indices, std::vector<Texture> textures)
	: m_Vertices(vertics), m_Indices(indices), m_Textures(textures)
{
	SetupMesh();
}

void Mesh::Draw(Shader& shader)
{
	// 绑定纹理
	uint32_t diffuseNr = 1;
	uint32_t specularNr = 1;
	uint32_t normalNr = 1;
	uint32_t heightNr = 1;

	for (uint32_t i = 0; i < m_Textures.size(); i++)
	{
		glActiveTexture(GL_TEXTURE0 + i);

		std::string name;
		TEXTURE_TYPE type = m_Textures[i].GetType();

		switch (type)
		{
		case TEXTURE_TYPE::DIFFUSE:
		{
			name = "u_DiffuseTexture" + std::to_string(diffuseNr++);
			break;
		}
		case TEXTURE_TYPE::SPECULAR:
		{
			name = "u_SpecularTexture" + std::to_string(specularNr++);
			break;
		}
		case TEXTURE_TYPE::NORMAL:
		{
			name = "u_NormalTexture" + std::to_string(normalNr++);
			break;
		}
		case TEXTURE_TYPE::HEIGHT:
		{
			name = "u_HeightTexture" + std::to_string(heightNr++);
			break;
		}
		}

		glUniform1i(glGetUniformLocation(shader.GetRendererID(), name.c_str()), i);
		glBindTexture(GL_TEXTURE_2D, m_Textures[i].GetRendererID());
	}

	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, static_cast<uint32_t>(m_Indices.size()), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
	glActiveTexture(GL_TEXTURE0);
}

void Mesh::SetupMesh()
{
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VBO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glBufferData(GL_ARRAY_BUFFER, m_Vertices.size() * sizeof(Vertex), &m_Vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_Indices.size() * sizeof(uint32_t), &m_Indices[0], GL_STATIC_DRAW);

	// 顶点位置属性
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), static_cast<void*>(0));

	// 顶点法向量属性
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>((offsetof(Vertex, Normal))));

	// 顶点纹理坐标属性
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>((offsetof(Vertex, TexCoords))));

	// 顶点切线属性
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>((offsetof(Vertex, Tangent))));

	// 顶点双切线属性
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>((offsetof(Vertex, BiTangent))));

	// 骨骼ID
	glEnableVertexAttribArray(5);
	glVertexAttribPointer(5, 4, GL_INT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>((offsetof(Vertex, m_BoneIDs))));

	// 骨骼权重
	glEnableVertexAttribArray(6);
	glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>((offsetof(Vertex, m_Weights))));

	glBindVertexArray(0);

}
