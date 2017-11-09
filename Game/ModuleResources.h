#pragma once

#ifndef __MODULE_RESOURCES_H__
#define __MODULE_RESOURCES_H__

#include "Module.h"

#include "GameObject.h"
#include "ComponentMaterial.h"
#include "ComponentMesh.h"

class ModuleResources : public Module {
public:
	ModuleResources(Application* parent, bool start_enabled = true);
	~ModuleResources();
public:
	bool Init();
	bool Start();
	update_status PreUpdate();
	update_status Update();
	update_status PostUpdate();
	bool CleanUp();

	double GenerateUID() const;

public:
	std::vector<ComponentMesh*> meshes;
	std::vector<ComponentMaterial*> materials;
	std::vector<Texture*> textures;

	std::vector<GameObject*> gameObjects;
};

#endif // !__MODULE_RESOURCES_H__
