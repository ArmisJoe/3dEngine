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

#define MESH_OWN_FORMAT "mymesh"
#define NUM_MESH_IDX 3

class ModuleAssimp : public Module {

public:
	ModuleAssimp(bool start_enabled = true);
	~ModuleAssimp();

public:
	//Scene
	GameObject* LoadGeometry(const char* path, const unsigned int pprocess_flag = aiProcessPreset_TargetRealtime_MaxQuality);
	//Nodes
	GameObject* LoadNode(const aiNode* node, const aiScene* scene, GameObject* parent = nullptr);
	GameObject* LoadNode(const aiNode* node, const aiScene* scene, const char* raw_path, GameObject* parent = nullptr);
	//Components
	ComponentMesh* LoadRawMesh(const aiMesh* m);
	ComponentMaterial* LoadMaterial(const aiMaterial* mat);
	//Importer
	bool Import(const aiMesh* m, std::string& output_file);
	bool SaveToOwnFormat(ComponentMesh* m, std::string& output_file); // [FORMAT] -> { nV, nI, nUV, Vs, Is, UVs }
	ComponentMesh* LoadMyFormatMesh(const char* exported_file);
	ComponentMesh* LoadToOwnFormat(const char* path, const uint pprocess_flag = aiProcessPreset_TargetRealtime_MaxQuality); // DO NOT USE PLS
	ComponentMesh* LoadToOwnFormat(const aiMesh* m);
	ComponentMesh* LoadToOwnFormat(const aiMesh* m, std::string& output_file);
	bool CleanUp();

};

#endif // !__MODULE_ASSIMP_H__
