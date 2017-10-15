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
	bool Start();
	update_status PreUpdate(float dt);
	update_status Update(float dt);
	update_status PostUpdate(float dt);
	bool CleanUp();
public:
	GameObject* AddGameObject(); //Adds a GameObject into Scene. Return nullptr on failure.
	GameObject* AddGameObject(GameObject* go); //Adds a GameObject into Scene. Return nullptr on failure.
	void RemoveAllGameObject();
public:
	std::list<GameObject*> gameObjects;

private:
};
