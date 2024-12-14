#pragma once

struct ShadowStruct
{
	unsigned int FBO;
	unsigned int depthMap;
	unsigned int depthCubemap;

};

const unsigned int SHADOW_WIDTH = 1024, SHADOW_HEIGHT = 1024;

ShadowStruct setup_shadowmap();
void RenderShadow();
void saveShadowMapToBitmap(unsigned int Texture, int w, int h);
