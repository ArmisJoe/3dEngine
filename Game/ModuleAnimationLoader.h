#pragma once
#ifndef __MODULE_ANIMATIONLOADER_H__
#define __MODULE_ANIMATIONLOADER_H__

#include "Module.h"

#include "ResourceAnimation.h"
#include "Assimp\include\anim.h"

#define EXTENSION_ANIMATIONS "myanim"

class ModuleAnimationLoader : public Module {
public:
	ModuleAnimationLoader(bool start_enabled = true);
	~ModuleAnimationLoader();
public:	// Module::
	bool Init();
	bool Start();
	update_status PreUpdate();
	update_status Update();
	update_status PostUpdate();
	bool CleanUp();
public: // AnimationLoader::
	bool Import(const aiAnimation* anim, std::string& output_file);
	bool Load(const char* file, Animation* res);
	bool Save(const Animation& anim, std::string& output_file);
	Animation* ImportToLoad(aiAnimation* anim);

private:
	AnimNode* ImportBone(const aiNodeAnim* node);
};

#endif // !__MODULE_ANIMATIONLOADER_H__
