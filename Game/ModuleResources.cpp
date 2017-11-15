#include "ModuleResources.h"

#include "Application.h"


ModuleResources::ModuleResources(bool start_enabled) : Module(start_enabled)
{
	name = "Resources";
}

ModuleResources::~ModuleResources()
{
}

bool ModuleResources::Init()
{
	return true;
}

bool ModuleResources::Start()
{
	return true;
}

update_status ModuleResources::PreUpdate()
{
	return UPDATE_CONTINUE;
}

update_status ModuleResources::Update()
{
	return UPDATE_CONTINUE;
}

update_status ModuleResources::PostUpdate()
{

	return UPDATE_CONTINUE;
}

bool ModuleResources::CleanUp()
{
	//Meshes CleanUp
	if (!meshes.empty()) {
		for (std::vector<ComponentMesh*>::iterator it = meshes.begin(); it != meshes.end(); it++) {
			if ((*it) != nullptr) {
				(*it)->CleanUp();
				mdelete(*it);
			}
		}
		meshes.clear();
	}
	LOG("All MESHES Cleaned Up");

	//Materials CleanUp
	if (!materials.empty()) {
		for (std::vector<ComponentMaterial*>::iterator it = materials.begin(); it != materials.end(); it++) {
			if ((*it) != nullptr) {
				(*it)->CleanUp();
				mdelete(*it);
			}
		}
		materials.clear();
	}
	LOG("All MATERIALS Cleaned Up");

	//Textures CleanUp
	if (!textures.empty()) {
		for (std::vector<Texture*>::iterator it = textures.begin(); it != textures.end(); it++) {
			if ((*it) != nullptr) {
				mdelete(*it);
			}
		}
	}
	LOG("All Textures Cleaned Up");

	// GO NOT REMOVE HERE - THEY ARE REMOVED WITH THE SCENE (go only exist on the scene)
	//f (!gameObjects.empty()) {
	//	for (int i = 0; i < gameObjects.size(); i++) {
	//		if (gameObjects[i] != nullptr) {
	//			gameObjects[i]->CleanUp();
	//			mdelete gameObjects[i];
	//		}
	//	}
	//

	return true;
}

double  ModuleResources::GenerateUID() const
{
	return RandomNumber();
}
