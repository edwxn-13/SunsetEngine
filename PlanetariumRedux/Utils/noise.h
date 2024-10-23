#pragma once

#include <cstdlib> 
#include <ctime> 
#include <iostream>
#include "obj.h"
#include "../Maths/MathHeader.h"
float randomFloat(float min, float max) 
{

	max = max + 100;
	srand((unsigned)time(0));
	float result = (float)((rand() % (int)max) + 1) / 100;
	return result;
}

float adjust(int index) 
{
	return glm::sin(randomFloat(0, index));
}

int random(int index)
{
	int forsep = index + 10;
	int res = forsep + (rand() % 100);
	return res;
}


class PerlinClass 
{
	float* noiseSeed;
	float* perlinNoise;

	int nOutput = 256;
	float scalingBias = 0.4f;


	float* noiseSeed2D = nullptr;
	float* perlinNoise2D =  nullptr;

	int outputWidth = 4000;
	int outputHeight = 4000;

	int nMode = 1;
public:

	PerlinClass()
	{
		
	}

	PerlinClass(int octave, float scaling_mode)
	{
		onUserCreate2D();
		scalingBias = scaling_mode;
		PerlinNoise2D(outputWidth, outputHeight, noiseSeed2D, octave, scalingBias, perlinNoise2D);
	}

	float getNoise(float x, float y) 
	{
		int x_c = x * outputWidth / 10;
		int y_c = y * outputWidth / 10;

		return perlinNoise2D[x_c * y_c];
	}

	void discard() 
	{
		free(noiseSeed2D);
		free(perlinNoise2D);

	}

	virtual bool onUserCreate() 
	{
		noiseSeed = new float[nOutput];
		perlinNoise = new float[nOutput];

		for (int i = 0; i < nOutput; i++) { noiseSeed[i] = (float)rand() / (float)RAND_MAX; }

		
		return true;
	}

	virtual bool onUserCreate2D()
	{
		noiseSeed2D = new float[outputWidth * outputHeight];
		perlinNoise2D = new float[outputWidth * outputHeight];

		for (int i = 0; i < outputWidth * outputHeight; i++) { noiseSeed2D[i] = (float)rand() / (float)RAND_MAX; }


		return true;
	}

	void PerlinNoise2D(int width, int height, float* seed, int octave, float fBias, float* output)
	{
		for (int x = 0; x < width; x++)
			for (int y = 0; y < height; y++)
			{
				float fNoise = 0.0f;
				float fScale = 100.0f;
				float fScaleeAcc = 0.0f;


				for (int o = 0; o < octave; o++)
				{
					int nPitch = width >> o;
					int nSampleX1 = (x / nPitch) * nPitch;
					int nSampleY1 = (x / nPitch) * nPitch;

					int nSampleX2 = (nSampleX1 + nPitch) % width;
					int nSampleY2 = (nSampleX2 + nPitch) % width;

					float pitchBendX = (float)(x - nSampleX1) / (float)nPitch;
					float pitchBendY = (float)(y - nSampleY1) / (float)nPitch;

					float fSampleT = (1.0f - pitchBendX) * seed[nSampleY1 * width + nSampleX1] + pitchBendX * seed[nSampleY1 * width + nSampleX2];
					float fSampleB = (1.0f - pitchBendX) * seed[nSampleY2 * width + nSampleX1] + pitchBendX * seed[nSampleY2 * width + nSampleX2];

					fNoise += pitchBendY * ((fSampleB - fSampleT) + fSampleT) * fScale;
					fScaleeAcc += fScale;
					fScale = fScale / fBias;
				}

				output[y * width + x] = fNoise / fScaleeAcc;
				//cout << output[y * width + x] << " ";
			}

		//cout << "\n" << " newline ";

	}
	void PerlinNoise1D(int nCount, float* seed, int octave, float fBias, float* output)
	{
		for (int x = 0; x < nCount; x++)
		{
			float fNoise = 0.0f;
			float fScale = 1.0f;
			float fScaleeAcc = 0.0f;


			for (int o = 0; o < octave; o++)
			{
				int nPitch = nCount >> o;
				int nSample1 = (x / nPitch) * nPitch;
				int nSample2 = (nSample1 + nPitch) % nCount;

				float pitchBend = (float)(x - nSample1) / (float)nPitch;
				float fSample = (1.0f - pitchBend) * seed[nSample1] + pitchBend * seed[nSample2];

				fNoise += fSample * fScale;
				fScaleeAcc += fScale;
				fScale = fScale / fBias;
			}

			output[x] = fNoise/fScaleeAcc;
		}
	}
};

vec3 randheight(float noise, vec3 sufNorm) 
{
	vec3 result = vec3(0,0,0);
	//glm::perlin();
	vec3 surface = sufNorm;
	float multi =  1 + noise;
	result.x =  (sufNorm.x * multi) * 0.1;
	result.y =  (sufNorm.y * multi) * 0.1;
	result.z =  (sufNorm.z * multi) * 0.1;
	return result;
}
