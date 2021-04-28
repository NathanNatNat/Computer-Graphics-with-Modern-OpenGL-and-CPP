// This is an independent project of an individual developer. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "Model.h"

Model::Model()
{

}

void Model::RenderModel()
{
	for (size_t i = 0; i < MeshList.size(); i++)
	{
		unsigned int MaterialIndex = MeshToTex[i];

		if (MaterialIndex < TextureList.size() && TextureList[MaterialIndex])
		{
			TextureList[MaterialIndex]->UseTexture();
		}

		MeshList[i]->RenderMesh();
	}
}

void Model::LoadModel(const std::string& fileName)
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(fileName, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenSmoothNormals | aiProcess_JoinIdenticalVertices);

	if (!scene)
	{
		printf("Model (%s) failed to load: %s", fileName, importer.GetErrorString());
		return;
	}

	LoadNode(scene->mRootNode, scene);

	LoadMaterials(scene);
}

void Model::LoadNode(aiNode* node, const aiScene* scene)
{
	for (size_t i = 0; i < node->mNumMeshes; i++)
	{
		LoadMesh(scene->mMeshes[node->mMeshes[i]], scene);
	}

	for (size_t i = 0; i < node->mNumChildren; i++)
	{
		LoadNode(node->mChildren[i], scene);
	}
}

void Model::LoadMesh(aiMesh* mesh, const aiScene* scene)
{
	std::vector<GLfloat> vertices;
	std::vector<unsigned int> indices;

	for (size_t i = 0; i < mesh->mNumVertices; i++)
	{
		vertices.insert(vertices.end(), { mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z });
		if (mesh->mTextureCoords[0])
		{
			vertices.insert(vertices.end(), { mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y });
		}
		else {
			vertices.insert(vertices.end(), { 0.0f, 0.0f });
		}
		vertices.insert(vertices.end(), { -mesh->mNormals[i].x, -mesh->mNormals[i].y, -mesh->mNormals[i].z });
	}

	for (size_t i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		for (size_t j = 0; j < face.mNumIndices; j++)
		{
			indices.push_back(face.mIndices[j]);
		}
	}

	Mesh* newMesh = new Mesh();
	newMesh->CreateMesh(&vertices[0], &indices[0], vertices.size(), indices.size());
	MeshList.push_back(newMesh);
	MeshToTex.push_back(mesh->mMaterialIndex);
}

void Model::LoadMaterials(const aiScene* scene)
{
	TextureList.resize(scene->mNumMaterials);

	for (size_t i = 0; i < scene->mNumMaterials; i++)
	{
		aiMaterial* Material = scene->mMaterials[i];

		TextureList[i] = nullptr;

		if (Material->GetTextureCount(aiTextureType_DIFFUSE))
		{
			aiString path;
			if (Material->GetTexture(aiTextureType_DIFFUSE, 0, &path) == AI_SUCCESS)
			{
				int idx = std::string(path.data).rfind("\\");
				std::string filename = std::string(path.data).substr(idx + 1);

				std::string texPath = std::string("Textures/"+ filename);

				TextureList[i] = new Texture(texPath.c_str());

				if (!TextureList[i]->LoadTexture())
				{
					printf("Failed to load texture at: %s\n", texPath);
					delete TextureList[i];
					TextureList[i] = nullptr;
				}
			}
		}

		if (!TextureList[i])
		{
			TextureList[i] = new Texture("Textures/plain.png");
			TextureList[i]->LoadTextureA();
		}
	}
}

void Model::ClearModel()
{
	for (size_t i = 0; i < MeshList.size(); i++)
	{
		if (MeshList[i])
		{
			delete MeshList[i];
			MeshList[i] = nullptr;
		}
	}

	for (size_t i = 0; i < TextureList.size(); i++)
	{
		if (TextureList[i])
		{
			delete TextureList[i];
			TextureList[i] = nullptr;
		}
	}
}

Model::~Model()
{

}
