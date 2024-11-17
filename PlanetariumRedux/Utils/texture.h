#pragma once

class SCamera;

#define WIDTH 1024
#define HEIGHT 768

float skyboxVerts[];
unsigned int skyboxIndicies[];


struct Texture
{
	const char* filename;
	int width, height, channels;
	unsigned char* pxls;
	unsigned int tex_id;
	Texture(const char* filename);
	Texture(const char* filename, int width, int height, int channels, unsigned char* pxls);
};

class TextureManager 
{
	static std::vector<Texture*> loadedFiles;

public:
	static void addToTextureList(Texture* tex);
	static Texture * checkForTexture(const char* filename);
};

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
