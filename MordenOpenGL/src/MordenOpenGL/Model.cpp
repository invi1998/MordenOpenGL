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

// ģ�ͼ���
void Model::LoadModel()
{
	Assimp::Importer importer;
	// ͨ���趨aiProcess_Triangulate�����Ǹ���Assimp�����ģ�Ͳ��ǣ�ȫ��������������ɣ�����Ҫ��ģ�����е�ͼԪ��״�任Ϊ�����Ρ�
	// aiProcess_FlipUVs���ڴ����ʱ��תy����������꣨����ܻ��ǵ�����������̳���˵������OpenGL�д󲿷ֵ�ͼ���y�ᶼ�Ƿ��ģ�����������ڴ���ѡ����޸������
	const aiScene* scene = importer.ReadFile(m_Filepath.c_str(), aiProcess_Triangulate | aiProcess_FlipUVs);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		std::cout << "ERROR::ASSIMP::" << importer.GetErrorString() << std::endl;
		return;
	}

	m_Directory = m_Filepath.substr(0, m_Filepath.find_last_of('/'));
	ProcessNode(scene->mRootNode, scene);

}


// ����ڵ�����
void Model::ProcessNode(aiNode* node, const aiScene* scene)
{
	// ����ڵ����е���������еĻ���
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		m_MeshVec.push_back(ProcessMesh(mesh, scene));
	}
	// �������������ӽڵ��ظ���һ����
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

	// ����ÿһ�����񶥵�
	for(unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		Vertex vertex;
		// ��������һ��ռλ��������Ϊassimpʹ���Լ��������࣬����ֱ��ת��Ϊglm��vec3�࣬���������Ƚ����ݴ��䵽���ռλ��glm::vec3�С�
		glm::vec3 vector3;
		// ����λ��
		vector3.x = mesh->mVertices[i].x;
		vector3.y = mesh->mVertices[i].y;
		vector3.z = mesh->mVertices[i].z;
		vertex.Position = vector3;
		// ���㷨��
		if (mesh->HasNormals())
		{
			vector3.x = mesh->mNormals[i].x;
			vector3.y = mesh->mNormals[i].y;
			vector3.z = mesh->mNormals[i].z;
			vertex.Normal = vector3;
		}
		// ��������
		if (mesh->mTextureCoords[0])
		{
			glm::vec2 vector2;
			// һ��������԰������8����ͬ���������ꡣ��ˣ����Ǽ������ǲ���ʹ��һ�������ж�����������ģ�ͣ���������ʼ�ղ��õ�һ������(0)��
			vector2.x = mesh->mTextureCoords[0][i].x;
			vector2.y = mesh->mTextureCoords[0][i].y;
			vertex.TexCoords = vector2;
			// ����
			vector3.x = mesh->mTangents[i].x;
			vector3.y = mesh->mTangents[i].y;
			vector3.z = mesh->mTangents[i].z;
			vertex.Tangent = vector3;
			// ˫����
			vector3.x = mesh->mBitangents[i].x;
			vector3.y = mesh->mBitangents[i].y;
			vector3.z = mesh->mBitangents[i].z;
			vertex.BiTangent = vector3;
		}
		else
		{
			aiFace face = mesh->mFaces[i];
			// ������������������������Ǵ洢�����������С�
			for (unsigned int j = 0; j < face.mNumIndices; j++)
				indices.push_back(face.mIndices[j]);
		}

		// �������
		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
		// ���Ǽ�����ɫ�������в��������Ƶ�Լ����ÿ������������Ӧ����Ϊ��texture_diffuseN����
		// ����N�Ǵ�1��MAX_SAMPLER_NUMBER���������֡�ͬ����������������
		// ������б��������һ�㣺
		// diffuse: texture_diffuseN
		// specular: texture_specularN
		// normal: texture_normalN

		// 1:����������
		std::vector<Texture> diffuseMaps = LoadMaterialTextures(material, aiTextureType_DIFFUSE, TEXTURE_TYPE::DIFFUSE);
		textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
		// 2:�߹���ͼ
		std::vector<Texture> specularMaps = LoadMaterialTextures(material, aiTextureType_SPECULAR, TEXTURE_TYPE::SPECULAR);
		textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
		// 3:������ͼ
		std::vector<Texture> normalMaps = LoadMaterialTextures(material, aiTextureType_NORMALS, TEXTURE_TYPE::NORMAL);
		textures.insert(textures.end(), normalMaps.begin(), normalMaps.end());
		// 4:�����ͼ
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
		// ��������Ƿ��Ѿ����أ�����ǣ��������һ�ε�������������������
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
