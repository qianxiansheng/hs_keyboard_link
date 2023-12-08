#include "layout.h"

#include "glad/glad/glad.h"

// initial static member
KLWindowLayoutManager* KLWindowLayoutManager::s_Instance = nullptr;
std::once_flag KLWindowLayoutManager::s_OnceFlag;

KLDialogManager* KLDialogManager::s_Instance = nullptr;
std::once_flag KLDialogManager::s_OnceFlag;

void* CreateTexture(uint8_t* data, int w, int h, char fmt)
{
	GLuint tex;
	glGenTextures(1, &tex);
	glBindTexture(GL_TEXTURE_2D, tex);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, w, h, 0, (fmt == 0) ? GL_BGRA : GL_RGBA, GL_UNSIGNED_BYTE, data);
	glBindTexture(GL_TEXTURE_2D, 0);

	return VOID_PTR_CAST(tex);
}

void DeleteTexture(void* tex)
{
	GLuint texID = (GLuint)((uintptr_t)tex);
	glDeleteTextures(1, &texID);
}

KLDialogManager::KLDialogManager()
{
	// File dialog adapter
	ifd::FileDialog::Instance().CreateTexture = [](uint8_t* data, int w, int h, char fmt) -> void* {
		return CreateTexture(data, w, h, fmt);
	};
	ifd::FileDialog::Instance().DeleteTexture = [](void* tex) {
		DeleteTexture(tex);
	};
}