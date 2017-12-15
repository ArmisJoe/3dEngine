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
	current_selected = nullptr;
	return ret;
}

update_status ModuleScene::PreUpdate(float dt)
{
	//SetAllToGlobalTransforms();

	if (root != nullptr)
		root->RemoveIteration(false);

	return UPDATE_CONTINUE;
}

// Update
update_status ModuleScene::Update(float dt)
{
	if (root != nullptr)
		root->Update(dt);


	/*float3 corners[8];
	ComponentCamera* tmp = (ComponentCamera*)App->camera->curr_camera;
	tmp->GetCorners(corners);
	App->renderer3D->debugger->DrawFrustum(corners);*/

	//App->renderer3D->debugger->SetColor(Red);
	IteratingElement(root);
	dynamic_gos_OnScreen.clear();
	return UPDATE_CONTINUE;
}

bool ModuleScene::IteratingElement(GameObject * go)
{
	for (std::vector<GameObject*>::iterator sub_it = go->children.begin(); sub_it < go->children.end(); ++sub_it)
	{
		if ((*sub_it)->HasAABB == true) {
			ComponentMesh* mesh = (ComponentMesh*)(*sub_it)->FindComponents(componentType_Mesh)[0];
			if ((*sub_it)->AABBneedsUpdate)
			{
				(*sub_it)->UpdateAABBFromMesh(mesh);
				(*sub_it)->AABBneedsUpdate = false;
			}
			App->renderer3D->debugger->DrawAABB((*sub_it)->aabb.CenterPoint(), (*sub_it)->aabb.Size());
			if ((*sub_it)->IsStatic() == false)
			{
				CollisionType col = App->camera->curr_camera->GetFrustum().ContainsBox((*sub_it)->aabb);
				if (col != OUTSIDE)
				{
					App->renderer3D->AddGameObjectToDraw((*sub_it));
					dynamic_gos_OnScreen.push_back((*sub_it));
				}
			}
		}
		IteratingElement(*sub_it);
	}
	return true;
}



// Load assets
bool ModuleScene::CleanUp()
{
	LOG("Unloading Scene");

	RemoveAllGameObject();
	root->CleanUp();
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
		parent->AddChild(newGameObject);
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
	new_go->SetStatic(true);
	App->res->gameObjects.push_back(new_go);
	return new_go;
}

GameObject * ModuleScene::AddGameObject(GameObject * parent, GameObject * go, bool byref)
{
	GameObject* new_go = nullptr;

	if (parent == nullptr || go == nullptr)
		return nullptr;

	if (byref == true) {
		new_go = go;
	}
	else {
		new_go = new GameObject(parent);
		memcpy(new_go, go, sizeof(GameObject));
	}

	if (new_go != nullptr) {
		new_go->SetParent(parent);
		new_go->SetScene(this);
		parent->AddChild(new_go);
	}

	return new_go;
}

void ModuleScene::DeleteGameObject(GameObject * go)
{
	if (go != nullptr) {
		go->GetParent()->WantToRemoveChild(go);
	}
}

void ModuleScene::SetSelected(GameObject * go)
{
	if (go != nullptr)
	{
		if (current_selected != nullptr)
			current_selected->selected = false;

		current_selected = go;
		current_selected->selected = true;

		if (App->editor->inspector != nullptr)
			App->editor->inspector->SetInspected(current_selected);
	}
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
	if (grid_on) {
		bPlane p(0, 1, 0, 0);
		p.axis = true;
		p.Render();
	}
	return UPDATE_CONTINUE;
}

const char* ModuleScene::Serialize(const char* scene_name) {
	App->fs->CreateFolder("Assets", "Scenes");
	std::string file = "Assets/Scenes/";
	file += scene_name;
	std::string test = GetExtension(scene_name);
	if(strcmp(test.c_str(), "json") != 0)
		file += ".json";

	JSON_Doc* scene_doc = App->parson->LoadJSON(file.c_str());

	scene_doc->SetString("scene.name", scene_name);

	scene_doc->SetArray("gameobjects");
	scene_doc->SetArray("components");
	if (root != nullptr) {
		root->Serialize(scene_doc);
	}

	scene_doc->Save();

	return file.c_str();

}

const char* ModuleScene::LoadScene(const char* scene_name, bool hasExtension)
{
	std::string file = "Assets/Scenes/";
	file += scene_name;
	if (strcmp(GetCExtension(scene_name), "json") != 0 && !hasExtension)
		file += ".json";

	JSON_Doc* scene_doc = App->parson->LoadJSON(file.c_str(), false);

	if (scene_doc == nullptr) {
		LOG("ERROR Loading Scene");
		return "";
	}

	RemoveAllGameObject();  // RESET SCENE
	App->quadTree->RestartQuadtree();
	App->renderer3D->GetToDraw().clear();
	// Name
	curr_scene_name = scene_doc->GetString("scene.name");
	// GameObjects Load
	std::vector<GameObject*> tmp_gos;
	scene_doc->MoveToRootSection();
	for (int i = 0; i < scene_doc->GetArraySize("gameobjects"); i++) {
		scene_doc->MoveToSectionInsideArr("gameobjects", i);
		if (scene_doc->GetBool("isRoot") == true) {
			root->SetUID(scene_doc->GetNumber("uid"));
			root->SetStatic(scene_doc->GetBool("static"));
			tmp_gos.push_back(root);
		}
		else {
			GameObject* go = CreateGameObject();
			if (go != nullptr) {
				go->SetUID(scene_doc->GetNumber("uid"));
				go->SetParentUID(scene_doc->GetNumber("parentUID"));
				go->SetName(scene_doc->GetString("name"));
				go->SetStatic(scene_doc->GetBool("static"));
				tmp_gos.push_back(go);
				go->SetStatic(true);
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
				checked->AddChild(curr);
				break;
			}
		}
	}
	// Components Load
	std::vector<Component*> tmp_cs;
	scene_doc->MoveToRootSection();
	int nComponents = scene_doc->GetArraySize("components");
	for (int i = 0; i < nComponents; i++) {
		scene_doc->MoveToRootSection();
		scene_doc->MoveToSectionInsideArr("components", i);
		int type = scene_doc->GetNumber("type");
		Component* c = nullptr;
		ComponentMaterial* mat = nullptr;
		ComponentMesh* cmesh = nullptr;
		int aux = 0;
		switch (type) {
		case componentType_Mesh:
			if(scene_doc->GetString("path") != nullptr)
			if (App->fs->exists(scene_doc->GetString("path")))
				cmesh = App->assimp->LoadMyFormatMesh(scene_doc->GetString("path"));
			else
				cmesh = App->assimp->LoadToOwnFormat(scene_doc->GetString("rawpath"));
			if (cmesh != nullptr) {
				cmesh->path = scene_doc->GetString("path");
				cmesh->raw_path = scene_doc->GetString("rawpath");
				App->res->meshes.push_back(cmesh);
				c = cmesh;
			}
			break;
		case componentType_Material:
			mat = new ComponentMaterial();
			aux = scene_doc->GetArraySize("texture_channels");
			for (int i = 0; i < aux; i++) {
				scene_doc->MoveToSectionInsideArr("texture_channels", i);
				if (App->fs->exists(scene_doc->GetString("path")))
					mat->SetTextureChannel((int)scene_doc->GetNumber("channel"), App->tex->LoadDDSTexture(scene_doc->GetString("path")));
				else
					mat->SetTextureChannel((int)scene_doc->GetNumber("channel"), App->tex->LoadToDDS(scene_doc->GetString("rawpath")));
			}
			if (mat != nullptr && mat->HasTextures()) {
				App->res->materials.push_back(mat);
				c = mat;
			}
			else
				if (mat != nullptr) {
					mat->CleanUp();
					mdelete mat;
				}
			scene_doc->MoveToRootSection();
			scene_doc->MoveToSectionInsideArr("components", i);
			break;
		case componentType_Transform:
			ComponentTransform* trans = new ComponentTransform();
			float3 p = { (float)scene_doc->GetNumber("position.x"), (float)scene_doc->GetNumber("position.y"), (float)scene_doc->GetNumber("position.z") };
			Quat r = { (float)scene_doc->GetNumber("rotation.x"), (float)scene_doc->GetNumber("rotation.y"), (float)scene_doc->GetNumber("rotation.z"), (float)scene_doc->GetNumber("rotation.w") };
			float3 s = { (float)scene_doc->GetNumber("scale.x"), (float)scene_doc->GetNumber("scale.y"), (float)scene_doc->GetNumber("scale.z") };
			trans->SetPosition(p);
			trans->SetQuatRotation(r);
			trans->SetScale(s);
			c = trans;
			break;
		}
		if (c != nullptr) {
			c->SetGoUID(scene_doc->GetNumber("parentUID"));
			c->type = (componentType)type;
			tmp_cs.push_back(c);
		}


	}
	// Components Link
	for (int i = 0; i < tmp_gos.size(); i++) {
		for (int k = 0; k < tmp_cs.size(); k++) {
			if (tmp_gos[i]->GetUID() == tmp_cs[k]->GetGoUID()) {
				tmp_gos[i]->AddComponent(tmp_cs[k]->type, tmp_cs[k], !(tmp_cs[k]->type == componentType_Transform));
			}
		}
	}
	// Reimporting obj to quadtree

	for (uint i = 0; i < tmp_gos.size(); ++i)
	{
		if (tmp_gos[i]->IsStatic())
		{
			App->quadTree->InsertObject(tmp_gos[i]);
		}
	}
	return file.c_str();

}
