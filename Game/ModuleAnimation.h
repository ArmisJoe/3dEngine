#pragma once
#ifndef __MODULE_ANIMATION_H__
#define __MODULE_ANIMATION_H__

#include "Module.h"
#include "GameObject.h"
#include "ComponentBone.h"

class ModuleAnimation : public Module {
public:
	ModuleAnimation(bool start_enabled = true);
	~ModuleAnimation();
public:	// Module::
	bool Init();
	bool Start();
	update_status PreUpdate();
	update_status Update();
	update_status PostUpdate();
	bool CleanUp();

private:
	void ResetAllDeformableMeshes(GameObject* go);
	void DeformMeshes(GameObject* go);
	void AdaptToBone(ComponentBone* bone);
};

#endif // MODULE_ANIMATION