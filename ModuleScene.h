#pragma once
#include "Module.h"
#include "Globals.h"
#include "Primitive.h"
#include "PhysBody3D.h"
#include "ModulePlayer.h"

#include "GameObject.h"

class ModuleScene : public Module
{
public:
	ModuleScene(Application* app, bool start_enabled = true);
	~ModuleScene();
	bool Init();
	bool Start();
	update_status PreUpdate(float dt);
	update_status Update(float dt);
	update_status PostUpdate(float dt);
	bool CleanUp();

public:
	GameObject* CreateGameObject(GameObject* parent); //Adds a GameObject into Parent. Return nullptr on failure.
	GameObject* AddGameObject(GameObject* parent, GameObject* go); //Adds a GameObject into a Parent GameObject. Return nullptr on failure.
	GameObject* GetRoot() const {
		return root;
	}
	void RemoveAllGameObject();

private:
	GameObject* root = nullptr;
};
