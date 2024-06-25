// This is an independent project of an individual developer. Dear PVS-Studio, please check it.

// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com

#include "Model.h"
#include <assimp/postprocess.h>
#include <iostream>

Model::Model() = default;

void Model::RenderModel() const
{
	for (size_t i = 0; i < MeshList.size(); i++)
	{
		const GLuint MaterialIndex = { MeshToTex[i] };

		if (MaterialIndex < TextureList.size() && TextureList[MaterialIndex])
		{
			TextureList[MaterialIndex]->UseTexture();
		}

		MeshList[i]->RenderMesh();
	}
}

void Model::LoadModel(const std::string & fileName)
{
	Assimp::Importer importer{ };
	const aiScene* scene = { importer.ReadFile(fileName, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenSmoothNormals | aiProcess_JoinIdenticalVertices) };

	if (!scene)
	{
		std::cout << "Model: (" << fileName << ") failed to load: " << importer.GetErrorString() << '\n';
		return;
	}

	LoadNode(scene->mRootNode, scene);

	LoadMaterials(scene);
}

void Model::LoadNode(const aiNode * node, const aiScene * scene)
{
	for (GLuint i = 0; i < node->mNumMeshes; i++)
	{
		LoadMesh(scene->mMeshes[node->mMeshes[i]], scene);
	}

	for (GLuint i = 0; i < node->mNumChildren; i++)
	{
		LoadNode(node->mChildren[i], scene);
	}
}

void Model::LoadMesh(const aiMesh * mesh, const aiScene * scene)
{
	std::vector<GLfloat> Vertices{ };
	std::vector<GLuint> Indices{ };

	for (GLuint i = 0; i < mesh->mNumVertices; i++)
	{
		Vertices.insert(Vertices.end(), { mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z });
		if (mesh->mTextureCoords[0])
		{
			Vertices.insert(Vertices.end(), { mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y });
		}
		else {
			Vertices.insert(Vertices.end(), { 0.0f, 0.0f });
		}
		Vertices.insert(Vertices.end(), { -mesh->mNormals[i].x, -mesh->mNormals[i].y, -mesh->mNormals[i].z });
	}

	for (GLuint i = 0; i < mesh->mNumFaces; i++)
	{
		const aiFace Face = { mesh->mFaces[i] };
		for (GLuint j = 0; j < Face.mNumIndices; j++)
		{
			Indices.push_back(Face.mIndices[j]);
		}
	}

	Mesh* NewMesh = { new Mesh() };
	NewMesh->CreateMesh(Vertices.data(), Indices.data(), Vertices.size(), Indices.size());
	MeshList.push_back(NewMesh);
	MeshToTex.push_back(mesh->mMaterialIndex);
}

void Model::LoadMaterials(const aiScene * scene)
{
	TextureList.resize(scene->mNumMaterials);
	
	for (GLuint i = 0; i < scene->mNumMaterials; i++)
	{
		const aiMaterial* Material = { scene->mMaterials[i] };

		TextureList[i] = { nullptr };

		if (Material->GetTextureCount(aiTextureType_DIFFUSE))
		{
			aiString Path{ };
			if (Material->GetTexture(aiTextureType_DIFFUSE, 0, &Path) == AI_SUCCESS)
			{
				const size_t Idx = { std::string(Path.data).rfind('\\') };
				std::string Filename = { std::string(Path.data).substr(Idx + 1) };

				std::string TexPath = { std::string("Textures/") + Filename };

				TextureList[i] = { new Texture(TexPath.c_str()) };

				if (!TextureList[i]->LoadTexture())
				{
					std::cout << "Failed to load texture at: " << TexPath << '\n';

					delete TextureList[i];
					TextureList[i] = { nullptr };
				}
			}
		}

		if (!TextureList[i])
		{
			TextureList[i] = { new Texture("Textures/plain.png") };
			TextureList[i]->LoadTextureA();
		}
	}
}

void Model::ClearModel()
{
	for (auto& i : MeshList)
	{
		if (i)
		{
			delete i;
			i = { nullptr };
		}
	}

	for (auto& i : TextureList)
	{
		if (i)
		{
			delete i;
			i = { nullptr };
		}
	}
}

Model::~Model() = default;
