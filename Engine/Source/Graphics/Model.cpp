/*// -----------------------------------------------------------------
*  File:		Model.cpp
*  Brief:		Implementation of models, vertices and meshes
*  Creation:	04/11/2022
*  Last Update:	04/11/2022
*
*  © 2022 Adrian Montes. All right reserved
// -----------------------------------------------------------------*/
#include <pch.h>
#include "Model.hpp"

/// -----------------------------------------------------------------
/// Vertex custom constructor
/// -----------------------------------------------------------------
Engine::Vertex::Vertex(vec3 pos, vec2 uv, vec3 normal)
{
	mPos = pos;
	mUV = uv;
	mNormal = normal;
}

/// -----------------------------------------------------------------
/// Mesh custom constructor
/// -----------------------------------------------------------------
Engine::Mesh::Mesh(std::vector<Vertex> vtxs, std::vector<unsigned> idxs) : mVertices(vtxs), mIndices(idxs)
{
	CreateBuffers();
}

/// -----------------------------------------------------------------
/// Renders mesh
/// -----------------------------------------------------------------
void Engine::Mesh::Render()
{
	glBindVertexArray(mVAO);
	glDrawElements(GL_TRIANGLES, static_cast<GLsizei>(mIndices.size()), GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}

/// -----------------------------------------------------------------
/// Creates buffers of mesh
/// -----------------------------------------------------------------
void Engine::Mesh::CreateBuffers()
{
	//Generate buffers
	glGenVertexArrays(1, &mVAO);
	glGenBuffers(1, &mVBO);
	glGenBuffers(1, &mIBO);

	//Set data to buffers
	glBindVertexArray(mVAO);
	glBindBuffer(GL_ARRAY_BUFFER, mVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(Vertex) * mVertices.size(), mVertices.data(), GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mIBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(unsigned) * mIndices.size(), mIndices.data(), GL_STATIC_DRAW);

	//Assign attribute of each part of vertices
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), 0);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Vertex::mUV));
	glEnableVertexAttribArray(1);
	glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, Vertex::mNormal));
	glEnableVertexAttribArray(2);

	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

}

/// -----------------------------------------------------------------
/// Default model constructor
/// -----------------------------------------------------------------
Engine::Model::Model()
{
	SetModel("cube.obj");
}

/// -----------------------------------------------------------------
/// Renders all the meshes of a model
/// -----------------------------------------------------------------
void Engine::Model::Render()
{
	for (auto mesh : mMeshes)
		mesh.Render();
}

/// -----------------------------------------------------------------
/// Sets model to given filename
/// -----------------------------------------------------------------
void Engine::Model::SetModel(std::string name)
{
	mMeshes.clear();
	std::string full_path = "../data/" + name;
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(full_path, aiProcess_Triangulate | aiProcess_FlipUVs);

	//If wrongly loaded, assert
	assert(scene);
	assert(!(scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE));
	assert(scene->mRootNode);

	//Load all meshes
	LoadMeshes(scene->mRootNode, scene);
}

/// -----------------------------------------------------------------
/// Gets meshes from model
/// -----------------------------------------------------------------
const std::vector<Engine::Mesh>& Engine::Model::GetMeshes() const
{
	return mMeshes;
}

/// -----------------------------------------------------------------
/// Loads meshes read from file
/// -----------------------------------------------------------------
void Engine::Model::LoadMeshes(const aiNode* node, const aiScene* scene)
{
	//Create all the meshes from this node
	for (unsigned i = 0; i < node->mNumMeshes; i++)
		mMeshes.push_back(CreateMesh(scene->mMeshes[node->mMeshes[i]]));

	//Go to children nodes
	for (unsigned i = 0; i < node->mNumChildren; i++)
		LoadMeshes(node->mChildren[i], scene);
}

/// -----------------------------------------------------------------
/// Creates engine mesh from importer mesh data
/// -----------------------------------------------------------------
const Engine::Mesh& Engine::Model::CreateMesh(const aiMesh* mesh)
{
	std::vector<Vertex> vtxs;
	std::vector<unsigned> idxs;

	//Setup all vertices
	for (unsigned i = 0; i < mesh->mNumVertices; i++)
	{
		Vertex vtx;
		const aiVector3D& aiPos = mesh->mVertices[i];
		const aiVector3D& aiUV = mesh->mNormals[i];
		const aiVector3D& aiNormal = mesh->mTextureCoords[0][i];
		vtx.mPos = vec3(aiPos.x, aiPos.y, aiPos.z);
		vtx.mNormal = vec3(aiNormal.x, aiNormal.y, aiNormal.z);

		if(mesh->mTextureCoords[0])
			vtx.mUV = vec2(aiUV.x, aiUV.y);

		vtxs.push_back(vtx);
	}

	//Setup indexes
	for (unsigned i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		for (unsigned j = 0; j < face.mNumIndices; j++)
			idxs.push_back(face.mIndices[j]);
	}

	//Create mesh
	return Mesh(vtxs, idxs);
}
