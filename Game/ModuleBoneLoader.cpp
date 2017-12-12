#include "ModuleBoneLoader.h"
#include "ResourceBone.h"
#include "Assimp/include/mesh.h"

#include "Application.h"

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
	return true;
}

bool ModuleBoneLoader::Start()
{
	return true;
}

update_status ModuleBoneLoader::PreUpdate(float dt)
{
	return UPDATE_CONTINUE;
}

update_status ModuleBoneLoader::Update(float dt)
{
	return UPDATE_CONTINUE;
}

update_status ModuleBoneLoader::PostUpdate(float dt)
{
	return UPDATE_CONTINUE;
}

bool ModuleBoneLoader::CleanUp()
{
	return true;
}

bool ModuleBoneLoader::Import(const aiBone * bone, std::string & output_file)
{
	bool ret = false;

	if (bone == nullptr) // Non-Valid Bone
		return false;

	// ok how do I store the componentmesh ?¿?!?!?

	ResourceBone new_bone;
	new_bone.name = bone->mName.C_Str();

	memcpy(new_bone.offsetMat.v, &bone->mOffsetMatrix.a1, sizeof(float) * 16);
	new_bone.num_weigths = bone->mNumWeights;

	new_bone.weigths = new float[bone->mNumWeights];
	new_bone.indices = new uint[bone->mNumWeights];
	

	for (uint i = 0; i < bone->mNumWeights; i++) {
		new_bone.indices[i] = bone->mWeights[i].mVertexId;
		
		new_bone.weigths[i] = bone->mWeights[i].mWeight;
	}

	ret = Save(new_bone, output_file);

	if (new_bone.weigths != nullptr) {
		mdelete[] new_bone.weigths;
	}
	if (new_bone.indices != nullptr) {
		mdelete[] new_bone.indices;
	}

	return ret;
}

bool ModuleBoneLoader::Load(const char * file, ResourceBone * res)
{
	bool ret = false;
	if (res == nullptr)
	{
		LOG("BonesLoad::Load() -> invalid file path");
		return false;
	}

	char* buffer = nullptr;
	uint buffer_size = App->fs->Load(file, &buffer);

	if (buffer == nullptr)
		return false;
	if (buffer_size <= 0)
		return false;

	char* it = buffer;

	// need to load componentmesh
	// [Ns N NWs Ts Is Ws]

	//name size
	uint name_size;
	memcpy(&name_size, it, sizeof(name_size));
	it += sizeof(name_size);

	//name
	char* tmp_name = new char[name_size + 1];
	memcpy(tmp_name, it, name_size);
	memcpy(&tmp_name[name_size], "\0", 1);
	res->name = tmp_name;
	mdelete[] tmp_name;
	it += name_size;
	//number of weights
	memcpy(&res->num_weigths, it, sizeof(res->num_weigths));
	it += sizeof(res->num_weigths);
	//trans
	memcpy(&res->offsetMat, it, sizeof(res->offsetMat));
	it += sizeof(res->offsetMat);
	//alloc
	uint size = name_size + sizeof(res->num_weigths) + sizeof(res->offsetMat) 
		+ sizeof(uint) * res->num_weigths + sizeof(float) * res->num_weigths;
	
	res->indices = new uint[res->num_weigths];
	res->weigths = new float[res->num_weigths];

	//indices
	memcpy(res->indices, it, sizeof(res->indices) * res->num_weigths);
	it += sizeof(res->indices) * res->num_weigths;
	//weights
	memcpy(res->weigths, it, sizeof(res->weigths) * res->num_weigths);
	it += sizeof(res->weigths) * res->num_weigths;


	if (buffer != nullptr)
		mdelete[] buffer;

	ret = true;

	return ret;
}

bool ModuleBoneLoader::ImportToLoad(const aiBone * bone, ResourceBone * res)
{
	bool ret = true;

	if (bone == nullptr || res == nullptr)
		return false;

	std::string file;

	if (Import(bone, file)) {
		if (!Load(file.c_str(), res))
			ret = false;
	}
	else
		ret = false;

	return ret;
}

bool ModuleBoneLoader::Save(const ResourceBone & bone, std::string & output_file)
{
	bool ret = false;

		// componentMesh missing...

		// Buffer Format
		// [Ns N NWs Ts Is Ws]
	uint name_size = sizeof(char) * bone.name.length();
	uint n_weights_size = sizeof(bone.num_weigths);
	uint trans_size = sizeof(bone.offsetMat);
	uint indices_size = sizeof(uint) * bone.num_weigths;
	uint weights_size = sizeof(float) * bone.num_weigths;

	uint buffer_size = name_size + n_weights_size + trans_size + indices_size + weights_size;
	// Buffer Alloc
	// Reminder: Buffer Format
	// [Ns N NWs Ts Is Ws]

	char* buffer = new char[buffer_size];
	char* it = buffer;

	// Name Size
	memcpy(it, &name_size, sizeof(name_size));
	it += sizeof(name_size);
	// Name
	const char* tmp_name = bone.name.c_str();
	memcpy(it, tmp_name, name_size);
	it += name_size;

	// Number of Wights
	memcpy(it, &bone.num_weigths, n_weights_size);
	it += n_weights_size;

	// Trans
	memcpy(it, &bone.offsetMat, trans_size);
	it += trans_size;

	//Indices
	memcpy(it, bone.indices, indices_size);
	it += indices_size;

	//Weights
	memcpy(it, bone.weigths, weights_size);
	it += weights_size;

	// And FINALLY, we actually save the file... :S
	ret = App->fs->SaveUnique(LIBRARY_BONES, buffer, bone.name.c_str(), EXTENSION_BONES, buffer_size, output_file);

	//if (buffer != nullptr)
	//	mdelete[] buffer;

	return ret;
}



