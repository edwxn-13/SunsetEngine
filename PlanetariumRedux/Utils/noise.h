#pragma once

#include <cstdlib> 
#include <ctime> 
#include <iostream>

class PerlinClass 
{
	float* noiseSeed;
	float* perlinNoise;

	int nOutput = 256;
	float scalingBias = 0.4f;


	float* noiseSeed2D = nullptr;
	float* perlinNoise2D =  nullptr;

	int outputWidth = 2000;
	int outputHeight = 2000;

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
		int x_c = x * outputWidth;
		int y_c = y * outputHeight;

		return perlinNoise2D[y_c * outputWidth + x_c];
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
				//std::cout << output[y * width + x] << " ";
			}

		//std::cout << "\n" << " newline ";

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
