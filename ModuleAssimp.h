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
	std::list<Mesh*> LoadGeometry(const char* path, const unsigned int pprocess_flag = aiProcessPreset_TargetRealtime_MaxQuality);
	void GenerateVerticesBuffer(const Mesh & mesh);
	void GenerateIndicesBuffer(const Mesh & mesh);

	bool CleanUp();

public:
	std::list<Mesh*> meshes;

};

#endif // !__MODULE_ASSIMP_H__
