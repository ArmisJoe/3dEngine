#pragma once
#include "Module.h"
#include "Globals.h"
#include "Primitive.h"
#include "PhysBody3D.h"
#include "ModulePlayer.h"
#include "ComponentCamera.h"
#include "GameObject.h"

class ModuleScene : public Module
{
	friend class ModuleEditorUI;
public:
	ModuleScene(bool start_enabled = true);
	~ModuleScene();
	bool Init();
	bool Start();
	update_status PreUpdate(float dt);
	update_status Update(float dt);
	update_status PostUpdate(float dt);
	bool CleanUp();

public:
	GameObject* CreateGameObject(GameObject* parent); //Adds a GameObject into Parent. Return nullptr on failure.
	GameObject* CreateGameObject();
	GameObject* AddGameObject(GameObject* parent, GameObject* go, bool byref = true); //Adds a GameObject into a Parent GameObject. Return nullptr on failure.
	void DeleteGameObject(GameObject* go);
	GameObject* GetRoot() const {
		return root;
	}
	GameObject* GetSelected() const {
		if (current_selected != nullptr)
		{
			return current_selected;
		}
		else return nullptr;
	}
	void SetSelected(GameObject* go);
	void RemoveAllGameObject();
	void SetAllToGlobalTransforms();
	bool IteratingElement(GameObject* go);

	const char* Serialize(const char* scene_name);
	const char* LoadScene(const char* scene_name, bool hasExtension = false);

	void SetGrid(bool flag) {
		grid_on = flag;
	}

public:
	std::string curr_scene_name = "scene01";
	vector<GameObject*> static_gos;
	vector<GameObject*> dynamic_gos_OnScreen;
private:
	GameObject* root = nullptr;
	GameObject* current_selected = nullptr;
protected:
	bool grid_on = true;
};
