#include "ModelParser.h"

#include <GL/gl3w.h>
#include <GLFW/glfw3.h>

#include <assimp/Importer.hpp>
#include <assimp/postprocess.h>

#include <iostream>
#include "../texture.h"
#include <assimp/scene.h>
#include <glm/gtc/type_ptr.hpp>

std::vector<STexture> textures_loaded = {};


Mesh::Mesh(std::vector<Vertex> vertices, std::vector<unsigned int> indices, std::vector<STexture> textures, SunsetMaterial mat)
{
    this->Material = mat;
    this->vertices = vertices;
    this->indices = indices;
    this->textures = textures;
    setupMesh();
}

void Mesh::setupMesh()
{
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    glBindVertexArray(VAO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);

    glBufferData(GL_ARRAY_BUFFER, vertices.size() * sizeof(Vertex), &vertices[0], GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(unsigned int),
        &indices[0], GL_STATIC_DRAW);

    // vertex positions
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)0);
    // vertex normals
    glEnableVertexAttribArray(1);
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, normal));
    // vertex texture coords
    glEnableVertexAttribArray(2);
    glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, sizeof(Vertex), (void*)offsetof(Vertex, uv));

    glBindVertexArray(0);
}

void Mesh::Draw(SunsetShader& shader, glm::mat4 position_matrix)
{
    unsigned int diffuseNr = 1;
    unsigned int specularNr = 1;
    for (unsigned int i = 0; i < textures.size(); i++)
    {
        glActiveTexture(GL_TEXTURE0 + i); 

        std::string number;
        std::string name = textures[i].type;
        if (name == "texture_diffuse")
            number = std::to_string(diffuseNr++);
        else if (name == "texture_specular")
            number = std::to_string(specularNr++);

        shader.setInt(("material." + name + number).c_str(), i);
        glBindTexture(GL_TEXTURE_2D, textures[i].id);
    }
    glActiveTexture(GL_TEXTURE0);

    // draw mesh
    glUniformMatrix4fv(glGetUniformLocation(shader.getProgram(), "model"), 1, GL_FALSE, glm::value_ptr(position_matrix));
    shader.setProperties(Material);
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, indices.size(), GL_UNSIGNED_INT, 0);
    glBindVertexArray(0);
}

void Model::Draw(SunsetShader& shader, glm::mat4 position_matrix)
{
    for (unsigned int i = 0; i < meshes.size(); i++)
        meshes[i].Draw(shader, position_matrix);
}

std::vector<Mesh> Model::getMeshInfo()
{
    return meshes;
}

void Model::processNode(aiNode* node, const aiScene* scene)
{
    // process all the node's meshes (if any)
    for (unsigned int i = 0; i < node->mNumMeshes; i++)
    {
        aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
        meshes.push_back(processMesh(mesh, scene));
    }
    // then do the same for each of its children
    for (unsigned int i = 0; i < node->mNumChildren; i++)
    {
        processNode(node->mChildren[i], scene);
    }
}

Mesh Model::processMesh(aiMesh* mesh, const aiScene* scene)
{
    std::vector<Vertex> vertices;
    std::vector<unsigned int> indices;
    std::vector<STexture> textures;
    SunsetMaterial mat;

    for (unsigned int i = 0; i < mesh->mNumVertices; i++)
    {
        Vertex vertex;
        Vector3f pos(mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z);
        Vector3f norms(mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z);
        Vector2f uvs;
        if(mesh->mTextureCoords[0])
        {
            uvs = Vector2f(mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y);
        }
        else { uvs = Vector2f(0, 0); }

        vertex.pos = pos;
        vertex.normal = norms;
        vertex.uv = uvs;
        vertices.push_back(vertex);
    }
    // process indices

    for (unsigned int i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];
        for (unsigned int j = 0; j < face.mNumIndices; j++)
            indices.push_back(face.mIndices[j]);
    }

    // process material
    if (mesh->mMaterialIndex >= 0)
    {
        aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];


        aiColor4D diffuse;
        aiGetMaterialColor(material, AI_MATKEY_COLOR_DIFFUSE, &diffuse);
        mat.diffuse = Vector3f(diffuse.r, diffuse.g, diffuse.b);

        aiColor4D specular;
        aiGetMaterialColor(material, AI_MATKEY_COLOR_SPECULAR, &specular);
        mat.specular = Vector3f(specular.r, specular.g, specular.b);

        float opacity;
        aiGetMaterialFloat(material, AI_MATKEY_OPACITY, &opacity);
        mat.opacity = 1.0f;

        float sheen;
        aiGetMaterialFloat(material, AI_MATKEY_SHININESS, &sheen);
        mat.sheen = sheen;

        std::vector<STexture> diffuseMaps = loadMaterialTextures(material,aiTextureType_DIFFUSE, "texture_diffuse");
        textures.insert(textures.end(), diffuseMaps.begin(), diffuseMaps.end());
        std::vector<STexture> specularMaps = loadMaterialTextures(material,aiTextureType_SPECULAR, "texture_specular");
        textures.insert(textures.end(), specularMaps.begin(), specularMaps.end());
        
    }

    return Mesh(vertices, indices, textures, mat);
}

std::vector<STexture> Model::loadMaterialTextures(aiMaterial* mat, aiTextureType type, std::string typeName)
{
    std::vector<STexture> textures;
    for (unsigned int i = 0; i < mat->GetTextureCount(type); i++)
    {
        aiString str;
        mat->GetTexture(type, i, &str);
        bool skip = false;
        for (unsigned int j = 0; j < textures_loaded.size(); j++)
        {
            if (std::strcmp(textures_loaded[j].path.data(), str.C_Str()) == 0)
            {
                textures.push_back(textures_loaded[j]);
                skip = true;
                break;
            }
        }
        if (!skip)
        {   // if texture hasn't been loaded already, load it
            STexture texture;
            //texture.id = CreateTexture(str.C_Str());
            TextureFromFile(str.C_Str(), this->directory);
            texture.type = typeName;
            texture.path = str.C_Str();
            textures.push_back(texture);
            textures_loaded.push_back(texture); // add to loaded textures
        }
    }
    return textures;
}

void Model::loadModel(std::string path)
{
    Assimp::Importer importer;
    const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate | aiProcess_FlipUVs);

    if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
    {
        //std::cout << "ERROR::ASSIMP::" << import.GetErrorString() <<std::endl;
        std::cout << "ERROR::ASSIMP::" <<std::endl;

        return;
    }
    directory = path.substr(0, path.find_last_of('/'));

    processNode(scene->mRootNode, scene);
}

