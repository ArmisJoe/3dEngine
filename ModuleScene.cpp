#include "Globals.h"
#include "Application.h"
#include "ModuleScene.h"
#include "Primitive.h"
#include "PhysBody3D.h"
#include "ModulePlayer.h"

ModuleScene::ModuleScene(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	name = "Scene";
}

ModuleScene::~ModuleScene()
{
}

// Load assets
bool ModuleScene::Start()
{
LOG("Loading Intro assets");
	bool ret = true;
	return ret;
}

// Load assets
bool ModuleScene::CleanUp()
{
	LOG("Unloading Scene");

	RemoveAllGameObject();

	return true;
}

GameObject * ModuleScene::AddGameObject()
{
	GameObject* newGameObject = nullptr;

	newGameObject = new GameObject();

	gameObjects.push_back(newGameObject);

	return newGameObject;
}

GameObject * ModuleScene::AddGameObject(GameObject * go)
{
	gameObjects.push_back(go);
	return go;
}

void ModuleScene::RemoveAllGameObject()
{
	if (!gameObjects.empty()) {
		for (std::list<GameObject*>::iterator it = gameObjects.begin(); it != gameObjects.end(); it++) {
			if ((*it) != nullptr) {
				(*it)->CleanUp();
				delete (*it);
			}
		}
		gameObjects.clear();
	}
}

update_status ModuleScene::PreUpdate(float dt)
{
	return UPDATE_CONTINUE;
}

// Update
update_status ModuleScene::Update(float dt)
{
	if (!gameObjects.empty()) {
		for (std::list<GameObject*>::iterator it = gameObjects.begin(); it != gameObjects.end(); it++) {
			if ((*it) != nullptr) {
				(*it)->Update(dt);
			}
		}
	}
	return UPDATE_CONTINUE;
}

update_status ModuleScene::PostUpdate(float dt)
{
	// Creates the Axis and Default Grid
	bPlane p(0, 1, 0, 0);
	p.axis = true;
	p.Render();

	return UPDATE_CONTINUE;
}
