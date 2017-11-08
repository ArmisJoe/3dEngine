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
	ModuleAssimp(bool start_enabled = true);
	~ModuleAssimp();

public:
	//Scene
	GameObject* LoadGeometry(const char* path, const unsigned int pprocess_flag = aiProcessPreset_TargetRealtime_MaxQuality);
	//Nodes
	GameObject* LoadNode(const aiNode* node, const aiScene* scene, GameObject* parent = nullptr);
	//Components
	ComponentMesh* LoadMesh(const aiMesh* m);
	ComponentMaterial* LoadMaterial(const aiMaterial* mat);

	bool CleanUp();

};

#endif // !__MODULE_ASSIMP_H__
