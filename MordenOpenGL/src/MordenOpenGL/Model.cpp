#include "hzpch.h"
#include "Model.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

Model::Model(const std::string& filePath, bool gamma) : m_Filepath(filePath), m_GammaCorrection(gamma)
{
	LoadModel();
}

void Model::Draw(Shader& shader)
{
	for (auto& mesh : m_MeshVec)
	{
		mesh.Draw(shader);
	}
}

// 模型加载
void Model::LoadModel()
{
	Assimp::Importer importer;
	// 通过设定aiProcess_Triangulate，我们告诉Assimp，如果模型不是（全部）由三角形组成，它需要将模型所有的图元形状变换为三角形。
	// aiProcess_FlipUVs将在处理的时候翻转y轴的纹理坐标（你可能还记得我们在纹理教程中说过，在OpenGL中大部分的图像的y轴都是反的，所以这个后期处理选项将会修复这个）
	const aiScene* scene = importer.ReadFile(m_Filepath.c_str(), aiProcess_Triangulate | aiProcess_FlipUVs);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
		return;
	}

	m_Directory = m_Filepath.substr(0, m_Filepath.find_last_of('/'));
	ProcessNode(scene->mRootNode, scene);

}


// 处理节点网络
void Model::ProcessNode(aiNode* node, const aiScene* scene)
{
	// 处理节点所有的网格（如果有的话）
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		m_MeshVec.push_back(ProcessMesh(mesh, scene));
	}
	// 接下来对它的子节点重复这一过程
	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		ProcessNode(node->mChildren[i], scene);
	}
}

Mesh Model::ProcessMesh(aiMesh* mesh, const aiScene* scene)
{
	std::vector<Vertex> vertices;
	std::vector<uint32_t> indices;
	std::vector<Texture> textures;

	// 遍历每一个网格顶点
	for(unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		Vertex vertex;
		// 我们声明一个占位向量，因为assimp使用自己的向量类，不能直接转换为glm的vec3类，所以我们先将数据传输到这个占位符glm::vec3中。
		glm::vec3 vector3;
		// 顶点位置
		vector3.x = mesh->mVertices[i].x;
		vector3.y = mesh->mVertices[i].y;
		vector3.z = mesh->mVertices[i].z;
		vertex.Position = vector3;
		// 顶点法线
		if (mesh->HasNormals())
		{
			vector3.x = mesh->mNormals[i].x;
			vector3.y = mesh->mNormals[i].y;
			vector3.z = mesh->mNormals[i].z;
			vertex.Normal = vector3;
		}
		// 纹理坐标
		if (mesh->mTextureCoords[0])
		{
			glm::vec2 vector2;
			// 一个顶点可以包含多达8个不同的纹理坐标。因此，我们假设我们不会使用一个顶点有多个纹理坐标的模型，所以我们始终采用第一个设置(0)。
			vector2.x = mesh->mTextureCoords[0][i].x;
			vector2.y = mesh->mTextureCoords[0][i].y;
			vertex.TexCoords = vector2;
			// 切线
			vector3.x = mesh->mTangents[i].x;
			vector3.y = mesh->mTangents[i].y;
			vector3.z = mesh->mTangents[i].z;
			vertex.Tangent = vector3;
			// 双切线
			vector3.x = mesh->mBitangents[i].x;
			vector3.y = mesh->mBitangents[i].y;
			vector3.z = mesh->mBitangents[i].z;
			vertex.BiTangent = vector3;
		}
		else
		{
			aiFace face = mesh->mFaces[i];
			// 检索面的所有索引，并将它们存储在索引向量中”
			for (unsigned int j = 0; j < face.mNumIndices; j++)
				indices.push_back(face.mIndices[j]);
		}

		// 处理材质
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
		// 我们假设着色器程序中采样器名称的约定。每个漫反射纹理应命名为“texture_diffuseN”，
		// 其中N是从1到MAX_SAMPLER_NUMBER的连续数字。同样适用于其他纹理，
		// 下面的列表概括了这一点：
		// diffuse: texture_diffuseN
		// specular: texture_specularN
		// normal: texture_normalN

		// 1:漫反射纹理
		std::vector<Texture> diffuseMaps = LoadMaterialTextures(material, aiTextureType_DIFFUSE, TEXTURE_TYPE::DIFFUSE);
		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
		// 2:高光贴图
		std::vector<Texture> specularMaps = LoadMaterialTextures(material, aiTextureType_SPECULAR, TEXTURE_TYPE::SPECULAR);
		textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
		// 3:法线贴图
		std::vector<Texture> normalMaps = LoadMaterialTextures(material, aiTextureType_NORMALS, TEXTURE_TYPE::NORMAL);
		textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
		// 4:深度贴图
		std::vector<Texture> heightMaps = LoadMaterialTextures(material, aiTextureType_HEIGHT, TEXTURE_TYPE::HEIGHT);
		textures.insert(textures.end(), heightMaps.begin(), heightMaps.end());

		return Mesh(vertices, indices, textures);

	}
}

std::vector<Texture> Model::LoadMaterialTextures(aiMaterial* mat, aiTextureType type, TEXTURE_TYPE typePre)
{
	std::vector<Texture> textures;
	for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
	{
		aiString str;
		mat->GetTexture(type, i, &str);
		// 检查纹理是否已经加载，如果是，则继续下一次迭代：跳过加载新纹理”
		bool skip = false;
		for (unsigned int j = 0; j < m_TextureLoaded.size(); j++)
		{
			if (std::strcmp(m_TextureLoaded[j].GetFilePath().data(), str.C_Str()) == 0)
			{
				textures.push_back(m_TextureLoaded[j]);
				skip = true;
				break;
			}
		}
		if (!skip)
		{
			Texture texture(str.C_Str(), typePre);
			textures.push_back(texture);
			m_TextureLoaded.push_back(texture);
		}
	}
	return textures;
}
