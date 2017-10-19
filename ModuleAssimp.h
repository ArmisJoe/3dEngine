#pragma once

#ifndef __MODULE_ASSIMP_H__
#define __MODULE_ASSIMP_H__

#include "Module.h"

#include "Assimp/include/cimport.h"
#include "Assimp/include/scene.h"
#include "Assimp/include/postprocess.h"
#include "Assimp/include/cfileio.h"

#pragma comment (lib, "Assimp/libx86/assimp.lib")

#include "GameObject.h"

class ModuleAssimp : public Module {

public:
	ModuleAssimp(Application* app, bool start_enabled = true);
	~ModuleAssimp();

public:
	std::vector<ComponentMesh*> LoadGeometry(const char* path, const unsigned int pprocess_flag = aiProcessPreset_TargetRealtime_MaxQuality);
	void GenerateVerticesBuffer(const ComponentMesh & mesh);
	void GenerateIndicesBuffer(const ComponentMesh & mesh);

	bool CleanUp();

public:
	std::vector<ComponentMesh*> meshes;

};

#endif // !__MODULE_ASSIMP_H__
