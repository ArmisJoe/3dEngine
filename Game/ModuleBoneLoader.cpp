#include "ModuleBoneLoader.h"
#include "ResourceBone.h"
#include "Assimp/include/mesh.h"

#include "mmgr/mmgr.h"

ModuleBoneLoader::ModuleBoneLoader(bool start_enabled) : Module(start_enabled)
{
	name = "Bone Loader";
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
	bool ret = false;

	if (bone == nullptr) // Non-Valid Bone
		return false;

	ResourceBone tmp_bone;

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



