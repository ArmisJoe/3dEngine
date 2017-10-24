#include "Globals.h"
#include "Application.h"
#include "ModuleScene.h"
#include "Primitive.h"
#include "PhysBody3D.h"
#include "ModulePlayer.h"

#include "ModuleEditorUI.h"
#include "PanelInspector.h"

ModuleScene::ModuleScene(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	name = "Scene";
}

ModuleScene::~ModuleScene()
{
}

bool ModuleScene::Init()
{
	LOG("Scene Init");

	root = new GameObject();

	return true;
}

// Load assets
bool ModuleScene::Start()
{
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

GameObject * ModuleScene::CreateGameObject(GameObject* parent)
{
	GameObject* newGameObject = nullptr;

	newGameObject = new GameObject();

	parent->children.push_back(newGameObject);

	return newGameObject;
}

GameObject * ModuleScene::AddGameObject(GameObject * parent, GameObject * go)
{
	GameObject* new_go = nullptr;

	if (parent != nullptr && go != nullptr) {
		new_go = new GameObject(parent);
		memcpy(new_go, go, sizeof(GameObject));
		new_go->SetParent(parent);
		new_go->SetScene(this);
		parent->children.push_back(new_go);
	}

	return new_go;
}

void ModuleScene::DeleteGameObject(GameObject * go)
{
	if (go != nullptr) {
		go->GetParent()->DeleteChild(go);
	}
}

void ModuleScene::SetSelected(GameObject * go)
{
	if(current_selected != nullptr)
		current_selected->selected = false;
	current_selected = go;
	current_selected->selected = true;

	if (App->editor->inspector != nullptr)
		App->editor->inspector->SetInspected(current_selected);

	LOG("HALO");
}

void ModuleScene::RemoveAllGameObject()
{
	if (root != nullptr) {
		root->CleanUp();
	}
}

update_status ModuleScene::PreUpdate(float dt)
{
	//[TEST] Assigning textures
	if (root != nullptr) {
		for (int i = 0; i < root->children.size(); i++) {
			for (int k = 0; k < root->children[i]->children.size(); k++) {
				std::vector<Component*> ms = root->children[i]->children[k]->FindComponents(componentType_Material);
				if (ms.empty()) {
					root->children[i]->children[k]->AddComponent(componentType_Material);
					ms = root->children[i]->children[k]->FindComponents(componentType_Material);
				}
				for (int it = 0; it < ms.size(); it++) {
					ComponentMaterial* mat = (ComponentMaterial*)ms[it];
					if (!App->res->textures.empty()) {
						if(mat->GetTextureChannel(texType_Diffuse) == nullptr)
							mat->SetTextureChannel(texType_Diffuse, App->res->textures.back());
					}
				}
			}
		}
	}
	//!_[TEST] Assigning textures
	return UPDATE_CONTINUE;
}

// Update
update_status ModuleScene::Update(float dt)
{
	if(root != nullptr)
		root->Update(dt);

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
