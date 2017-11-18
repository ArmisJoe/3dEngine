#pragma once

#ifndef __MODULE_TEXTURES_H__
#define __MODULE_TEXTURES_H__

#include "Module.h"
#include "GameObject.h"
#include "ComponentMaterial.h"

#define DEFAULT_TEXTURE_NAME "__default_texture_name__"


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
	ModuleTextures( bool start_enabled = true);
	~ModuleTextures();

public:
	bool Init();
	bool Start();
	update_status Update(float dt);

	bool CleanUp();

	Texture* LoadRawTexture(const char* path); // Returns nullptr on failure.
	int ImportTexture(const char* path, std::string& output_file); // RETURNS: -1 on already existing tex, 0 on failure and 1 on success. Imports a Texture into DDS format. Returns [false] on failure;
	Texture* LoadDDSTexture(const char* path); // Returns nullptr on failure
	Texture* LoadToDDS(const char* path, std::string& output_file);
	Texture* LoadToDDS(const char* path);
	void DrawConfigPanel();
	void LoadConfig();

private: // Importer Methods
	bool Import(const char* path, std::string& output_file); // Creates a DDS File from other extensions [NOT LOADING FILE INTO ENGINE]
	bool Import(const void* buffer, uint size, std::string& output_file, const char* file_name); // Creates a DDS File from other extensions [NOT LOADING FILE INTO ENGINE]
	Texture* Load(const char* exported_file);
	Texture* Load(const char* exported_file, Texture* tex); // fills 'tex' with the loaded texture

private: // Configurations
	int clamp_type = clampingTexType_ClampRepeat;
	int interpolation_type = interpolationTexType_Linear;
	char* clamping_str = nullptr;
	char* interpolate_str = nullptr;

private: // Loading Global Vars
	int curr_tex_state = false;

};

#endif // !__MODULE_TEXTURES_H__
