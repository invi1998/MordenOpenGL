#pragma once
#include <assimp/scene.h>

#include "MordenOpenGL/Mesh.h"

// 模型类
class Model
{
public:
	Model(const std::string& filePath, bool gamma);

	void Draw(Shader& shader);

	std::vector<Mesh> GetMeshs() const { return m_MeshVec; }

	std::vector<Texture> GetLoadedTexture() const { return m_TextureLoaded; }

private:
	void LoadModel();
	void ProcessNode(aiNode* node, const aiScene* scene);
	Mesh ProcessMesh(aiMesh* mesh, const aiScene* scene);

	std::vector<Texture> LoadMaterialTextures(aiMaterial* mat, aiTextureType type, TEXTURE_TYPE typePre);

private:
	// 模型数据
	std::vector<Texture> m_TextureLoaded;
	std::vector<Mesh> m_MeshVec;
	std::string m_Directory;
	std::string m_Filepath;
	bool m_GammaCorrection;		// 是否伽马校正
};

