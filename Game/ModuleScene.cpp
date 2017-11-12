#include "Globals.h"
#include "Application.h"
#include "ModuleScene.h"
#include "Primitive.h"
#include "PhysBody3D.h"
#include "ModulePlayer.h"

#include "ModuleEditorUI.h"
#include "PanelInspector.h"

#include "HelperFoos.h"

#include "Component.h"
#include "ComponentTransform.h"
#include "ComponentMesh.h"


ModuleScene::ModuleScene(bool start_enabled) : Module(start_enabled)
{
	name = "Scene";
}

ModuleScene::~ModuleScene()
{
}

bool ModuleScene::Init()
{
	LOG("Scene Init");

	root = new GameObject(nullptr);
	root->SetName("Scene Root");
	root->SetRoot(true);
	root->AddComponent(componentType_Transform);

	return true;
}

// Load assets
bool ModuleScene::Start()
{
	bool ret = true;

	return ret;
}

update_status ModuleScene::PreUpdate(float dt)
{
	//[TEST] Assigning textures
  /*
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
						if (mat->GetTextureChannel(texType_Diffuse) == nullptr)
							mat->SetTextureChannel(texType_Diffuse, App->res->textures.back());
					}
				}
			}
		}
	}
	//!_[TEST] Assigning textures*/
  //SetAllToGlobalTransforms();
	return UPDATE_CONTINUE;
}

// Update
update_status ModuleScene::Update(float dt)
{
	if (root != nullptr)
		root->Update(dt);

	float3 corners[8];
	ComponentCamera* tmp = (ComponentCamera*)App->camera->curr_camera;
	tmp->GetCorners(corners);
	App->renderer3D->debugger->DrawFrustum(corners);

	IteratingElement(root);


	return UPDATE_CONTINUE;
}

bool ModuleScene::IteratingElement(GameObject * go)
{
	for (std::vector<GameObject*>::iterator sub_it = go->children.begin(); sub_it < go->children.end(); ++sub_it)
	{
		if (!(*sub_it)->FindComponents(componentType_Mesh).empty()) {
			ComponentMesh* mesh = (ComponentMesh*)(*sub_it)->FindComponents(componentType_Mesh)[0];
			(*sub_it)->UpdateAABBFromMesh(mesh);
			App->renderer3D->debugger->DrawAABB((*sub_it)->aabb.CenterPoint(), (*sub_it)->aabb.Size());
		}
		CollisionType type = App->camera->curr_camera->GetFrustum().ContainsBox((*sub_it)->aabb);
		if (type != OUTSIDE)
		App->renderer3D->todraw.push_back((*sub_it));
		IteratingElement(*sub_it);
	}
	return true;
}



// Load assets
bool ModuleScene::CleanUp()
{
	LOG("Unloading Scene");

	RemoveAllGameObject();
	mdelete root;

	return true;
}

GameObject * ModuleScene::CreateGameObject(GameObject* parent)
{
	GameObject* newGameObject = nullptr;
	if (parent != nullptr) {
		newGameObject = new GameObject(parent);
		newGameObject->SetScene(this);
		newGameObject->SetParent(parent);
		parent->children.push_back(newGameObject);
	}
	else
		LOG("ERROR Trying to create a nullptr parent GameObject");
	return newGameObject;
}

GameObject * ModuleScene::CreateGameObject()
{
	GameObject* new_go = nullptr;
	new_go = new GameObject();
	new_go->SetScene(this);
	return new_go;
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
	//if(current_selected != nullptr)
	//	current_selected->selected = false;
	current_selected = go;
	//current_selected->selected = true;

	if (App->editor->inspector != nullptr)
		App->editor->inspector->SetInspected(current_selected);
}

void ModuleScene::RemoveAllGameObject()
{
	if (root != nullptr) {
		root->CleanUp();
	}
}

void ModuleScene::SetAllToGlobalTransforms()
{
	if (root != nullptr && !root->children.empty()) {
		root->SetToGlobalTransform();
	}
}
update_status ModuleScene::PostUpdate(float dt)
{
	// Creates the Axis and Default Grid
	bPlane p(0, 1, 0, 0);
	p.axis = true;
	p.Render();

	return UPDATE_CONTINUE;
}

void ModuleScene::Serialize(const char* scene_name) {
	App->fs->CreateFolder("Assets", "Scenes");
	std::string file = "Assets/Scenes/";
	file += scene_name;
	if(strcmp(GetCExtension(scene_name), ".json") != 0)
		file += ".json";

	JSON_Doc* scene_doc = App->parson->LoadJSON(file.c_str());

	scene_doc->SetString("scene.name", scene_name);

	scene_doc->SetArray("gameobjects");
	scene_doc->SetArray("components");
	if (root != nullptr) {
		root->Serialize(scene_doc);
	}

	scene_doc->Save();

}

void ModuleScene::LoadScene(const char* scene_name)
{
	std::string file = "Assets/Scenes/";
	file += scene_name;
	if (strcmp(GetCExtension(scene_name), ".json") != 0)
		file += ".json";

	JSON_Doc* scene_doc = App->parson->LoadJSON(file.c_str());

	if (scene_doc == nullptr) {
		LOG("ERROR Loading Scene");
		return;
	}

	RemoveAllGameObject();  // RESET SCENE

	// GameObjects Load
	std::vector<GameObject*> tmp_gos;
	scene_doc->MoveToRootSection();
	for (int i = 0; i < scene_doc->GetArraySize("gameobjects"); i++) {
		scene_doc->MoveToSectionInsideArr("gameobjects", i);
		if (scene_doc->GetBool("isRoot") == true) {
			root->SetUID(scene_doc->GetNumber("uid"));
			tmp_gos.push_back(root);
		}
		else {
			GameObject* go = CreateGameObject();
			if (go != nullptr) {
				go->SetUID(scene_doc->GetNumber("uid"));
				go->SetParentUID(scene_doc->GetNumber("parentUID"));
				go->SetName(scene_doc->GetString("name"));
				tmp_gos.push_back(go);
			}
			else
				LOG("ERROR Loading gameobject '%s'", scene_doc->GetString("name"));
		}
		scene_doc->MoveToRootSection();
	}
	// GameObjects Connections
	for (int i = 0; i < tmp_gos.size(); i++) {
		GameObject* curr = tmp_gos[i];
		for (int k = 0; k < tmp_gos.size(); k++) {
			GameObject* checked = tmp_gos[k];
			if (curr->GetParentUID() == checked->GetUID()) {
				curr->SetParent(checked);
				checked->children.push_back(curr);
				break;
			}
		}
	}
	// Components Load
	for (int n = 0; n < tmp_gos.size(); n++) {
		scene_doc->MoveToRootSection();
		for (int i = 0; i < scene_doc->GetArraySize("components"); i++) {
			scene_doc->MoveToRootSection();
			scene_doc->MoveToSectionInsideArr("components", i);
			if (scene_doc->GetNumber("parentUID") == tmp_gos[i]->GetParentUID()) {
				int compType = scene_doc->GetNumber("type");
				Component* newComp = nullptr;
				switch (compType) {
				case componentType_Mesh:
					break;
				case componentType_Material:
					break;
				case componentType_Transform:
					break;
				}
				tmp_gos[i]->AddComponent(compType, newComp, !(compType == componentType_Transform));
			}
		}
	}
}
