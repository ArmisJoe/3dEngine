#pragma once

#ifndef __MODULE_TEXTURES_H__
#define __MODULE_TEXTURES_H__

#include "Module.h"

struct Mesh;

struct Texture {
	int id = 0;
	float w = 0;
	float h = 0;

	int format = 0;
};

class ModuleTextures : public Module {
public:
	ModuleTextures(Application* app, bool start_enabled = true);
	~ModuleTextures();

public:
	bool Init();

	bool CleanUp();

	uint LoadTexture(const char* path);

public:
	std::list<Texture*> textures;

};

#endif // !__MODULE_TEXTURES_H__
