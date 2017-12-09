#pragma once
#ifndef __MODULE_BONELOADER_H__
#define __MODULE_BONELOADER_H__

#include "Module.h"

#include "ResourceBone.h"
#include "Application.h"

struct aiBone;

#define EXTENSION_BONES "mybone"

class ModuleBoneLoader : public Module {
public:
	ModuleBoneLoader(bool start_enabled = true);
	~ModuleBoneLoader();
public:	// Module::
	bool Init();
	bool Start();
	update_status PreUpdate();
	update_status Update();
	update_status PostUpdate();
	bool CleanUp();
public: // BoneLoader::
	bool Import(const aiBone* bone, std::string& output_file);
	bool Load(const char* file, ResourceBone* res);
	bool Save(const ResourceBone& bone, std::string& output_file);
};

#endif // !__MODULE_BONELOADER_H__
#pragma once
