#include "ModuleAnimationLoader.h"

#include "Application.h"
#include "ModuleFileSystem.h"

ModuleAnimationLoader::ModuleAnimationLoader(bool start_enabled) : Module(start_enabled)
{
	name = "Animation Loader";
}

ModuleAnimationLoader::~ModuleAnimationLoader()
{
}

bool ModuleAnimationLoader::Init()
{
	return true;
}

bool ModuleAnimationLoader::Start()
{
	return true;
}

update_status ModuleAnimationLoader::PreUpdate()
{
	return UPDATE_CONTINUE;
}

update_status ModuleAnimationLoader::Update()
{
	return UPDATE_CONTINUE;
}

update_status ModuleAnimationLoader::PostUpdate()
{
	return UPDATE_CONTINUE;
}

bool ModuleAnimationLoader::CleanUp()
{
	return true;
}

bool ModuleAnimationLoader::Import(const aiAnimation * anim, std::string & output_file)
{
	bool ret = false;
	
	if (anim == nullptr) // Non-Valid Anim
		return false;
	if (anim->mNumChannels < 0)	// No Bones in Anim
		return false;

	Animation new_anim;

	new_anim.name = anim->mName.C_Str();
	new_anim.duration = anim->mDuration;
	new_anim.tickspersec = anim->mTicksPerSecond;
	
	for (uint i = 0; i < anim->mNumChannels; i++) {
		Bone* new_bone = ImportBone(anim->mChannels[i]);
		if(new_bone != nullptr)
			new_anim.Channels.push_back(new_bone);
	}

	return Save(new_anim, output_file);
}

bool ModuleAnimationLoader::Save(const Animation & anim, std::string & output_file)
{
	bool ret = false;

	// Buffer Format
	// [Ns N Dur TpS Chnls[N nP nR nS P R S]]

	return ret;
}

Bone * ModuleAnimationLoader::ImportBone(const aiNodeAnim * node)
{
	if (node == nullptr)
		return nullptr;

	Bone* new_b = new Bone();

	new_b->name = node->mNodeName.C_Str();
	
	// Translation
	for (int i = 0; i < node->mNumPositionKeys; i++) {
		TransformKeys::VectorKey newPosKey;
		newPosKey.time = node->mPositionKeys[i].mTime;
		newPosKey.value.x = node->mPositionKeys[i].mValue.x;
		newPosKey.value.y = node->mPositionKeys[i].mValue.y;
		newPosKey.value.z = node->mPositionKeys[i].mValue.z;
		new_b->transKeys.positionKeys.push_back(newPosKey);
	}
	// Rotation
	for (int i = 0; i < node->mNumRotationKeys; i++) {
		TransformKeys::QuatKey newRotKey;
		newRotKey.time = node->mRotationKeys[i].mTime;
		newRotKey.value.x = node->mRotationKeys[i].mValue.x;
		newRotKey.value.y = node->mRotationKeys[i].mValue.y;
		newRotKey.value.z = node->mRotationKeys[i].mValue.z;
		newRotKey.value.w = node->mRotationKeys[i].mValue.w;
		new_b->transKeys.rotationKeys.push_back(newRotKey);
	}
	// Scaling
	for (int i = 0; i < node->mNumScalingKeys; i++) {
		TransformKeys::VectorKey newScaleKey;
		newScaleKey.time = node->mScalingKeys[i].mTime;
		newScaleKey.value.x = node->mScalingKeys[i].mValue.x;
		newScaleKey.value.y = node->mScalingKeys[i].mValue.y;
		newScaleKey.value.z = node->mScalingKeys[i].mValue.z;
		new_b->transKeys.scalingKeys.push_back(newScaleKey);
	}

	return new_b;
}
