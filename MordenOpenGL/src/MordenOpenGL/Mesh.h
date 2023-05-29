#pragma once

#include <glad/glad.h>
#include <glm/glm.hpp>

#include "MordenOpenGL/Shader.h"
#include "MordenOpenGL/Texture.h"

#define MAX_BONE_INFLUENCE 4


// 顶点结构体
struct  Vertex
{
	// 位置属性
	glm::vec3 Position;
	// 法线
	glm::vec3 Normal;
	// 纹理坐标属性
	glm::vec2 TexCoords;
	// 切线
	glm::vec3 Tangent;
	// 双切线
	glm::vec3 BiTangent;
	// 影响此顶点的骨骼索引
	int m_BoneIDs[MAX_BONE_INFLUENCE];
	// 骨骼权重
	float m_Weights[MAX_BONE_INFLUENCE];

};

// 网格类
class Mesh
{
public:
	Mesh(const std::vector<Vertex>& vertics, const std::vector<uint32_t>& indices, std::vector<Texture>& textures);
	~Mesh();
	void final() const;

	// 顶点渲染
	void Draw(Shader& shader);

private:
	void SetupMesh();

private:
	uint32_t VAO, VBO, EBO;

	std::vector<Vertex> m_Vertices;		// 顶点数据
	std::vector<uint32_t> m_Indices;	// 顶点索引数据
	std::vector<Texture> m_Textures;	// 纹理数据

};

