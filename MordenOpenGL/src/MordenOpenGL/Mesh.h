#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "MordenOpenGL/Shader.h"
#include "MordenOpenGL/Texture.h"

#define MAX_BONE_INFLUENCE 4


// ����ṹ��
struct  Vertex
{
	// λ������
	glm::vec3 Position;
	// ����
	glm::vec3 Normal;
	// ������������
	glm::vec2 TexCoords;
	// ����
	glm::vec3 Tangent;
	// ˫����
	glm::vec3 BiTangent;
	// Ӱ��˶���Ĺ�������
	int m_BoneIDs[MAX_BONE_INFLUENCE];
	// ����Ȩ��
	float m_Weights[MAX_BONE_INFLUENCE];

};

// ������
class Mesh
{
public:
	Mesh(const std::vector<Vertex>& vertics, const std::vector<uint32_t>& indices, std::vector<Texture>& textures);
	~Mesh();
	void final() const;

	// ������Ⱦ
	void Draw(Shader& shader);

private:
	void SetupMesh();

private:
	uint32_t VAO, VBO, EBO;

	std::vector<Vertex> m_Vertices;		// ��������
	std::vector<uint32_t> m_Indices;	// ������������
	std::vector<Texture> m_Textures;	// ��������

};

