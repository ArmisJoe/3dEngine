#pragma once

#ifndef __MODULE_TEXTURES_H__
#define __MODULE_TEXTURES_H__

#include "Module.h"
#include "GameObject.h"
#include "ComponentMaterial.h"

#include "ImporterTexture.h"

enum clampingTexType {
	clampingTexType_ClampToEdge = 0,
	clampingTexType_ClampRepeat,
	clampingTexType_ClampMirroredRepeat
};
enum interpolationTexType {
	interpolationTexType_Nearest = 0,
	interpolationTexType_Linear
};

class ModuleTextures : public Module {
public:
public:
	ModuleTextures(Application* app, bool start_enabled = true);
	~ModuleTextures();

public:
	bool Init();

	bool CleanUp();

	Texture* LoadRawTexture(const char* path); // Returns nullptr on failure.
	bool ImportTexture(const char* path, std::string& output_file); // Imports a Texture into DDS format. Returns [false] on failure;
	Texture* LoadDDSTexture(const char* path);
	void DrawConfigPanel();
public:
	ImporterTexture* importer = nullptr; // Texture Importer

private: // Configurations
	int clamp_type = clampingTexType_ClampRepeat;
	int interpolation_type = interpolationTexType_Linear;
	char* clamping_str = nullptr;
	char* interpolate_str = nullptr;

};

#endif // !__MODULE_TEXTURES_H__
