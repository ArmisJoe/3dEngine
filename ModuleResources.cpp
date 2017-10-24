#include "ModuleResources.h"

#include "Application.h"

ModuleResources::ModuleResources(Application * parent, bool start_enabled) : Module(parent, start_enabled)
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
				delete[](*it);
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
				delete[](*it);
			}
		}
		materials.clear();
	}
	LOG("All MATERIALS Cleaned Up");

	//Textures CleanUp
	if (!textures.empty()) {
		for (std::vector<Texture*>::iterator it = textures.begin(); it != textures.end(); it++) {
			if ((*it) != nullptr) {
				delete[](*it);
			}
		}
	}
	LOG("All Textures Cleaned Up");

	return true;
}