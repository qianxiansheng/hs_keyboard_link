#pragma once

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

struct StbUtils
{
	static unsigned char* load_file(const char* filename, int* width, int* height, int* channels, int req_comp)
	{
		return stbi_load(filename, width, height, channels, req_comp);
	}

	static void free_image(unsigned char* pData)
	{
		stbi_image_free(pData);
	}
};