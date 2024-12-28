#pragma once

#include "../../Maths/MathHeader.h"
#include "../../Engine/Renderer/ShaderManager/ShaderManager.h"

struct aiNode;
struct aiScene;
struct aiMaterial;
enum aiTextureType;
struct aiMesh;

#include <vector>
#include <string>

struct Vertex
{
	Vector3f pos;
	Vector3f normal;
	Vector2f uv;
};

struct STexture {
	unsigned int id;
	std::string type;
	std::string path;
};

struct Mesh 
{
	SunsetMaterial Material;
	std::vector<Vertex> vertices;
	std::vector<unsigned int> indices;
	std::vector<STexture> textures;

	void Draw(SunsetShader& shader, glm::mat4 position_matrix);

	Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<STexture> textures , SunsetMaterial mat);

private:
	unsigned int VBO, VAO, EBO;
	void setupMesh();
};


struct Model 
{
	Model(const char* path) 
	{
		loadModel(path);
	}

	void Draw(SunsetShader &shader, glm::mat4 position_matrix);
	std::vector<Mesh> getMeshInfo();

private:

	std::vector<Mesh> meshes;
	std::string directory;

	void processNode(aiNode* node, const aiScene* scene);
	Mesh processMesh(aiMesh* mesh, const aiScene* scene);
	std::vector<STexture> loadMaterialTextures(aiMaterial* mat, aiTextureType type,
		std::string typeName);

	void loadModel(std::string path);

};