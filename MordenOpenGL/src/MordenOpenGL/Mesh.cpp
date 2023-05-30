#include "hzpch.h"
#include "Mesh.h"

Mesh::Mesh(const std::vector<Vertex>& vertics, const std::vector<uint32_t>& indices, std::vector<Texture>& textures)
	: m_Vertices(vertics), m_Indices(indices), m_Textures(textures)
{
	/*m_Vertices.insert(m_Vertices.end(), vertics.begin(), vertics.end());
	m_Indices.insert(m_Indices.end(), indices.begin(), indices.end());
	m_Textures.insert(m_Textures.end(), textures.begin(), textures.end());*/

	SetupMesh();
}

Mesh::~Mesh()
{
	// ��Ҫ�������ͷ�VBO�ȿռ� ��ΪMesh���󴫵�ʱ ��ʱ�������ٺ����������VBO�ȿռ�
}

void Mesh::final() const
{
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
}

void Mesh::Draw(Shader& shader)
{
	// ������
	uint32_t diffuseNr = 1;
	uint32_t specularNr = 1;
	uint32_t normalNr = 1;
	uint32_t heightNr = 1;

	// shader.Use();
	// std::cout << "[-2]" << glGetError() << std::endl; // ���� 0 (�޴���

	for (uint32_t i = 0; i < m_Textures.size(); i++)
	{
		glActiveTexture(GL_TEXTURE0 + i);
		// std::cout << "[-1]" << glGetError() << std::endl; // ���� 0 (�޴���

		std::string name;
		TEXTURE_TYPE type = m_Textures[i].GetType();

		switch (type)
		{
		case TEXTURE_TYPE::DIFFUSE:
		{
			name = "u_DiffuseTexture" + std::to_string(diffuseNr++);
		}break;
		case TEXTURE_TYPE::SPECULAR:
		{
			name = "u_SpecularTexture" + std::to_string(specularNr++);
			
		}break;
		case TEXTURE_TYPE::NORMAL:
		{
			name = "u_NormalTexture" + std::to_string(normalNr++);
			
		}break;
		case TEXTURE_TYPE::HEIGHT:
		{
			name = "u_HeightTexture" + std::to_string(heightNr++);
			
		}break;
		}
		glUniform1i(glGetUniformLocation(shader.GetRendererID(), name.c_str()), static_cast<GLint>(i));

		//int activeTexture;
		//glGetIntegerv(GL_ACTIVE_TEXTURE, &activeTexture);
		//if (activeTexture == GL_TEXTURE0) {
		//	std::cout << "����Ԫ 0 �ǵ�ǰ���������Ԫ" << std::endl;
		//}
		//else if (activeTexture == GL_TEXTURE1) {
		//	std::cout << "����Ԫ 1 �ǵ�ǰ���������Ԫ" << std::endl;
		//}
		//else {
		//	std::cout << "��������Ԫ�ǵ�ǰ���������Ԫ" << std::endl;
		//}

		m_Textures[i].Bind(GL_TEXTURE_2D);
		// std::cout << "[1]" << glGetError() << '-' << m_Textures[i].GetRendererID() << '-' << std::endl; // ���� 0 (�޴���
	}

	// ��Ⱦ����
	glBindVertexArray(VAO);
	glDrawElements(GL_TRIANGLES, static_cast<unsigned int>(m_Indices.size()), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
	glActiveTexture(GL_TEXTURE0);
}

void Mesh::SetupMesh()
{
	glGenVertexArrays(1, &VAO);
	glGenBuffers(1, &VBO);
	glGenBuffers(1, &EBO);

	glBindVertexArray(VAO);
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	// �ṹ���һ���ܰ������������ǵ��ڴ沼�ֶ������г�Ա���������ġ�
	// ���Ч���������ǿ��Լ򵥵ش���һ���ṹ��ָ�룬��������ת��Ϊһ�� glm::vec3/2 ���飬��ת��Ϊ 3/2 ��������������ת��Ϊ�ֽ����� 
	glBufferData(GL_ARRAY_BUFFER, m_Vertices.size() * sizeof(Vertex), &m_Vertices[0], GL_STATIC_DRAW);

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, m_Indices.size() * sizeof(uint32_t), &m_Indices[0], GL_STATIC_DRAW);

	// ����λ������
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), static_cast<void*>(0));

	// ���㷨��������
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>((offsetof(Vertex, Normal))));

	// ����������������
	glEnableVertexAttribArray(2);
	glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>((offsetof(Vertex, TexCoords))));

	// ������������
	glEnableVertexAttribArray(3);
	glVertexAttribPointer(3, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>((offsetof(Vertex, Tangent))));

	// ����˫��������
	glEnableVertexAttribArray(4);
	glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>((offsetof(Vertex, BiTangent))));

	// ����ID
	glEnableVertexAttribArray(5);
	glVertexAttribIPointer(5, 4, GL_INT, sizeof(Vertex), reinterpret_cast<void*>((offsetof(Vertex, m_BoneIDs))));

	// ����Ȩ��
	glEnableVertexAttribArray(6);
	glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>((offsetof(Vertex, m_Weights))));

	// �ָ�Ĭ�ϵ�VAO����
	glBindVertexArray(0);

}
