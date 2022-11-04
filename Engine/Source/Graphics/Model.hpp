#pragma once

using glm::vec2;
using glm::vec3;
namespace Engine
{
	struct Vertex
	{
		Vertex() = default;
		Vertex(vec3 pos, vec2 uv, vec3 normal);
		vec3 mPos = vec3(0.f);
		vec2 mUV = vec2(0.f);
		vec3 mNormal = vec3(0.f);
	};

	class Mesh
	{
	public:
		Mesh(std::vector<Vertex> vtxs, std::vector<unsigned> idxs);
		void Render();

	private:
		std::vector<Vertex> mVertices;
		std::vector<unsigned> mIndices;
		GLuint mVAO = 0;
		GLuint mVBO = 0;
		GLuint mIBO = 0;
		void CreateBuffers();
	};

	class Model
	{
	public:
		Model();
		void Render();
		void SetModel(std::string name);
		const std::vector<Mesh>& GetMeshes() const;
		
	private:
		std::vector<Mesh> mMeshes;
		glm::vec3 mColor = vec3(1.f);

		//For assimp loader
		void LoadMeshes(const aiNode* node, const aiScene* scene);
		const Mesh& CreateMesh(const aiMesh* mesh);
	};
}
