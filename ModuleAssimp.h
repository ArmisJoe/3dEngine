#pragma once

#ifndef __MODULE_ASSIMP_H__
#define __MODULE_ASSIMP_H__

#include "Module.h"

#include "Assimp/include/cimport.h"
#include "Assimp/include/scene.h"
#include "Assimp/include/postprocess.h"
#include "Assimp/include/cfileio.h"

#pragma comment (lib, "Assimp/libx86/assimp.lib")

struct Texture;

struct Mesh {

	uint id_vertices = 0; // id in VRAM
	uint num_indices = 0;
	uint* indices = nullptr;

	uint id_indices = 0; // id in VRAM
	uint num_vertices = 0;
	float* vertices = nullptr;

	Texture* tex = nullptr; // Mesh Texture
	uint id_UV = 0; // id in VRAM
	float* textureCoords; // UV Coords
	uint num_UV = 0;

	float material_index; // material ID
	uint num_UVChannels = 0;
};

class ModuleAssimp : public Module {

public:
	ModuleAssimp(Application* app, bool start_enabled = true);
	~ModuleAssimp();

public:
	void LoadGeometry(const char* path, const unsigned int pprocess_flag = aiProcessPreset_TargetRealtime_MaxQuality);
	void GenerateVerticesBuffer(const Mesh & mesh);
	void GenerateIndicesBuffer(const Mesh & mesh);

	bool CleanUp();

public:
	std::list<Mesh*> meshes;

};

#endif // !__MODULE_ASSIMP_H__
