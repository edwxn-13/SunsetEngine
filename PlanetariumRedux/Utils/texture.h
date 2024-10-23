#pragma once

class SCamera;

#define WIDTH 1024
#define HEIGHT 768

float skyboxVerts[];
unsigned int skyboxIndicies[];

struct Skybox
{
	GLuint cubemapTex;
	unsigned int skyboxVBO, skyboxVAO, skyboxEBO;

	Skybox();

	GLuint loadCubemap();

	void setupCubemap();
	void renderCubemap(unsigned int skybox_shader, SCamera* Camera);
};

GLuint setup_texture(const char* filename);
GLuint setup_mipmaps(const char* filename[], int n);
GLuint CreateTexture(const char* filename);
