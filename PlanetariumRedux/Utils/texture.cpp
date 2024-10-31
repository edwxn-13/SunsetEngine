
#include <iostream>

#include <GL/glcorearb.h>
#include <gl/GL.h>
#include <GL/gl3w.h>
#include <GLFW/glfw3.h>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#include "../Camera/camera.h"
#include "texture.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"


std::vector<Texture*> TextureManager::loadedFiles = {};

unsigned int skyboxIndicies[] =
{
	//right 
	1,2,6,
	6,5,1,

	//left 
	0,4,7,
	7,3,0,

	//top
	4,5,6,
	6,7,4,

	//bottom
	0,3,2,
	2,1,0,

	//back
	0,1,5,
	5,4,0,

	//front
	3,7,6,
	6,2,3
};

float skyboxVerts[] =
{
	//   Coordinates
	-1.0f, -1.0f,  1.0f,//0        7--------6
	 1.0f, -1.0f,  1.0f,//1       /|       /|
	 1.0f, -1.0f, -1.0f,//2      4--------5 |
	-1.0f, -1.0f, -1.0f,//3      | |      | |
	-1.0f,  1.0f,  1.0f,//4      | 3------|-2
	 1.0f,  1.0f,  1.0f,//5      |/       |/
	 1.0f,  1.0f, -1.0f,//6      0--------1
	-1.0f,  1.0f, -1.0f//7
};

void TextureManager::addToTextureList(Texture* tex)
{
	loadedFiles.push_back(tex);
}

Texture* TextureManager::checkForTexture(const char* filename)
{
	for (int i = 0; i < loadedFiles.size(); i++)
	{
		if (strcmp(loadedFiles[i]->filename, filename)) { return loadedFiles[i]; }
	}

	return nullptr;
}

Texture::Texture(const char* filename)
{
	this->filename = filename;
	width = 0;
	height = 0;
	channels = 0;
}

Texture::Texture(const char* filename, int width, int height, int channels, unsigned char* pxls)
{
	this->filename = filename;
	this->width = width;
	this->height = height;
	this->channels = channels;
	this->pxls = pxls;
}


Skybox::Skybox()
{
	cubemapTex = loadCubemap();
}

GLuint Skybox::loadCubemap()
{
	glEnable(GL_TEXTURE_CUBE_MAP);

	std::string cubemapFaces[6] =
	{
		"cubemap/right.png",
		"cubemap/left.png",
		"cubemap/top.png",
		"cubemap/bottom.png",
		"cubemap/front.png",
		"cubemap/back.png"
	};
	GLuint cubemapTex;

	glGenTextures(1, &cubemapTex);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTex);

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);



	for (unsigned int i = 0; i < 6; i++)
	{
		int width, height, nrChannels;
		unsigned char* data = stbi_load(cubemapFaces[i].c_str(), &width, &height, &nrChannels, 0);
		if (data)
		{
			stbi_set_flip_vertically_on_load(false);
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
			stbi_image_free(data);

			std::cout << "succ" << std::endl;
		}
		else
		{
			std::cout << "fucc" << std::endl;
			stbi_image_free(data);

		}
	}
	glDisable(GL_BLEND);

	return cubemapTex;
}

void Skybox::setupCubemap()
{
	glGenVertexArrays(1, &skyboxVAO);
	glGenBuffers(1, &skyboxVBO);
	glGenBuffers(1, &skyboxEBO);
	glBindVertexArray(skyboxVAO);
	glBindBuffer(GL_ARRAY_BUFFER, skyboxVBO);
	glBufferData(GL_ARRAY_BUFFER, sizeof(skyboxVerts), &skyboxVerts, GL_STATIC_DRAW);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, skyboxEBO);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(skyboxIndicies), &skyboxIndicies, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindVertexArray(0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}

void Skybox::renderCubemap(unsigned int skybox_shader, SCamera* Camera)
{
	glDisable(GL_DEPTH_CLAMP);
	glDisable(GL_BLEND);
	glEnable(GL_CULL_FACE);
	glCullFace(GL_FRONT);
	glDepthFunc(GL_LEQUAL);

	glUseProgram(skybox_shader);

	glUniform1i(glGetUniformLocation(skybox_shader, "skybox"), 0);

	glm::mat4 view = glm::mat4(1.f);
	view = Camera->getSkyboxViewMatrix();
	glUniformMatrix4fv(glGetUniformLocation(skybox_shader, "camMat"), 1, GL_FALSE, glm::value_ptr(view));
	glActiveTexture(GL_TEXTURE0);
	glEnable(GL_TEXTURE_CUBE_MAP);
	glBindTexture(GL_TEXTURE_CUBE_MAP, cubemapTex);
	glBindVertexArray(skyboxVAO);
	glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

	glEnable(GL_BLEND);
	glDepthFunc(GL_LESS);
	glDisable(GL_CULL_FACE);
	glEnable(GL_DEPTH_CLAMP);
}

GLuint setup_texture(const char* filename)
{
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);

	GLuint texObject;
	glGenTextures(1, &texObject);
	glBindTexture(GL_TEXTURE_2D, texObject);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	int w, h, chan;
	unsigned char* pxls = stbi_load(filename, &w, &h, &chan, 0);
	stbi_set_flip_vertically_on_load(true);
	if (pxls)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, w, h, 0, GL_RGB, GL_UNSIGNED_BYTE, pxls);
		delete[] pxls;
	}

	glDisable(GL_TEXTURE_2D);
	glDisable(GL_BLEND);

	return texObject;
}




GLuint setup_mipmaps(const char* filename[], int n)
{
	glEnable(GL_TEXTURE_2D);
	glEnable(GL_BLEND);

	GLuint texObject;
	glGenTextures(1, &texObject);
	glBindTexture(GL_TEXTURE_2D, texObject);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	int w[16], h[16], chan[16];
	unsigned char* pxls[16];

	stbi_set_flip_vertically_on_load(true);

	for (int i = 0; i < n; i++) {


		pxls[i] = stbi_load(filename[i], &w[i], &h[i], &chan[i], 0);

		if (pxls[i])
		{

			glTexImage2D(GL_TEXTURE_2D, i, GL_RGB, w[i], h[i], 0, GL_RGB, GL_UNSIGNED_BYTE, pxls[i]);
			glGenerateMipmap(GL_TEXTURE_2D); // Generate mipmaps after each texture upload

		}
		delete[] pxls[i];
	}

	glDisable(GL_TEXTURE_2D);
	glDisable(GL_BLEND);


	return texObject;
}


GLuint CreateTexture(const char* filename)
{
	glEnable(GL_BLEND);

	GLuint texObject;
	glGenTextures(1, &texObject);
	glBindTexture(GL_TEXTURE_2D, texObject);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_LINEAR);

	Texture* temp_tex = new Texture(filename);

	Texture* tex = TextureManager::checkForTexture(filename);
	if (tex) 
	{
		temp_tex = tex;
	}

	else 
	{
		std::cout << "ok we loaded this bih\n";
		temp_tex->pxls = stbi_load(temp_tex->filename, &temp_tex->width, &temp_tex->height, &temp_tex->channels, 0);
		TextureManager::addToTextureList(temp_tex);
	}

	//---------------------------------------------------------------------------------------------------------------------------------

	if (temp_tex->pxls != NULL)
	{
		printf("Loaded %s\n", temp_tex->filename);
		if (temp_tex->channels == 4)
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, temp_tex->width, temp_tex->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, temp_tex->pxls);
		if (temp_tex->channels == 3)
			glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, temp_tex->width, temp_tex->height, 0, GL_RGB, GL_UNSIGNED_BYTE, temp_tex->pxls);
	}
	else
	{
		printf("Failed to load %s\n", temp_tex->filename);
	}

	glGenerateMipmap(GL_TEXTURE_2D);

	//delete[] temp_tex->pxls;

	glDisable(GL_BLEND);

	return texObject;
}

