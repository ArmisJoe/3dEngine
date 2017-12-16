#pragma once
#ifndef __MODULE_ANIMATION_H__
#define __MODULE_ANIMATION_H__

#include "Module.h"
#include "GameObject.h"
#include "ComponentBone.h"

class ModuleSkinning : public Module {
public:
	ModuleSkinning(bool start_enabled = true);
	~ModuleSkinning();
public:	// Module::
	bool Init();
	bool Start();
	update_status PreUpdate(float dt);
	update_status Update(float dt);
	update_status PostUpdate(float dt);
	bool CleanUp();

private:
	void ResetAllDeformableMeshes(GameObject* go);
public:
	void AdaptMeshToBone(ComponentBone* bone, ComponentMesh* mesh);
};

#endif // MODULE_ANIMATION