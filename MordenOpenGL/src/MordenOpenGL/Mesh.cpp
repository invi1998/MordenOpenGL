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
	// 不要再这里释放VBO等空间 因为Mesh对象传递时 临时对象销毁后这里会清理VBO等空间
}

void Mesh::final() const
{
	glDeleteVertexArrays(1, &VAO);
	glDeleteBuffers(1, &VBO);
	glDeleteBuffers(1, &EBO);
}

void Mesh::Draw(Shader& shader)
{
	// 绑定纹理
	uint32_t diffuseNr = 1;
	uint32_t specularNr = 1;
	uint32_t normalNr = 1;
	uint32_t heightNr = 1;

	// shader.Use();
	// std::cout << "[-2]" << glGetError() << std::endl; // 返回 0 (无错误

	for (uint32_t i = 0; i < m_Textures.size(); i++)
	{
		glActiveTexture(GL_TEXTURE0 + i);
		// std::cout << "[-1]" << glGetError() << std::endl; // 返回 0 (无错误

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
		//	std::cout << "纹理单元 0 是当前激活的纹理单元" << std::endl;
		//}
		//else if (activeTexture == GL_TEXTURE1) {
		//	std::cout << "纹理单元 1 是当前激活的纹理单元" << std::endl;
		//}
		//else {
		//	std::cout << "其他纹理单元是当前激活的纹理单元" << std::endl;
		//}

		m_Textures[i].Bind(GL_TEXTURE_2D);
		// std::cout << "[1]" << glGetError() << '-' << m_Textures[i].GetRendererID() << '-' << std::endl; // 返回 0 (无错误
	}

	// 渲染网格
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
	// 结构体的一个很棒的特性是它们的内存布局对于所有成员都是连续的。
	// 这个效果就是我们可以简单地传递一个结构体指针，它完美地转换为一个 glm::vec3/2 数组，再转换为 3/2 个浮点数，最终转换为字节数组 
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
	glVertexAttribIPointer(5, 4, GL_INT, sizeof(Vertex), reinterpret_cast<void*>((offsetof(Vertex, m_BoneIDs))));

	// 骨骼权重
	glEnableVertexAttribArray(6);
	glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, sizeof(Vertex), reinterpret_cast<void*>((offsetof(Vertex, m_Weights))));

	// 恢复默认的VAO环境
	glBindVertexArray(0);

}
