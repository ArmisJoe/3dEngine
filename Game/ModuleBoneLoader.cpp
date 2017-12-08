#include "ModuleBoneLoader.h"

ModuleBoneLoader::ModuleBoneLoader(bool start_enabled) : Module(start_enabled)
{
}

ModuleBoneLoader::~ModuleBoneLoader()
{
}

bool ModuleBoneLoader::Init()
{
	return false;
}

bool ModuleBoneLoader::Start()
{
	return false;
}

update_status ModuleBoneLoader::PreUpdate()
{
	return update_status();
}

update_status ModuleBoneLoader::Update()
{
	return update_status();
}

update_status ModuleBoneLoader::PostUpdate()
{
	return update_status();
}

bool ModuleBoneLoader::CleanUp()
{
	return false;
}

bool ModuleBoneLoader::Import(const aiBone * bone, std::string & output_file)
{
	return false;
}

bool ModuleBoneLoader::Load(const char * file, ResourceBone * res)
{
	return false;
}

bool ModuleBoneLoader::Save(const ResourceBone & bone, std::string & output_file)
{
	return false;
}

ResourceBone * ModuleBoneLoader::ImportToLoad(aiBone * bone)
{
	return nullptr;
}


