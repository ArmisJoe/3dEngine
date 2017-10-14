#pragma once

#ifndef __MODULE_TEXTURES_H__
#define __MODULE_TEXTURES_H__

#include "Module.h"
#include "GameObject.h"

class ModuleTextures : public Module {
public:
	ModuleTextures(Application* app, bool start_enabled = true);
	~ModuleTextures();

public:
	bool Init();

	bool CleanUp();

	Texture* LoadTexture(const char* path); // Returns nullptr on failure.

public:
	std::list<Texture*> textures;

};

#endif // !__MODULE_TEXTURES_H__
