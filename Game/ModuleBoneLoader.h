#pragma once
#ifndef __MODULE_BONELOADER_H__
#define __MODULE_BONELOADER_H__

#include "Module.h"

#include "ResourceBone.h"

struct aiBone;

#define EXTENSION_BONES "mybone"

class ModuleBoneLoader : public Module {
public:
	ModuleBoneLoader(bool start_enabled = true);
	~ModuleBoneLoader();
public:	// Module::
	bool Init();
	bool Start();
	update_status PreUpdate(float dt);
	update_status Update(float dt);
	update_status PostUpdate(float dt);
	bool CleanUp();
public: // BoneLoader::
	bool Import(const aiBone* bone, std::string& output_file);
	bool Load(const char* file, ResourceBone* res);
	bool ImportToLoad(const aiBone* bone, ResourceBone* res);

	bool Save(const ResourceBone& bone, std::string& output_file);
};

#endif // !__MODULE_BONELOADER_H__
#pragma once
