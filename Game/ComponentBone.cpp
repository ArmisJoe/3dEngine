#include "ComponentBone.h"

ComponentBone::ComponentBone() : Component(componentType_Bone)
{
	name = "Bone";
}

ComponentBone::ComponentBone(GameObject * argparent) : Component(componentType_Bone, argparent)
{
	name = "Bone";
}
 
ComponentBone::ComponentBone(componentType argtype, GameObject * argparent) : Component(componentType_Bone, argparent)
{
	name = "Bone";
}

ComponentBone::~ComponentBone()
{
}

void ComponentBone::Start()
{
}

void ComponentBone::Update(float dt)
{
}

void ComponentBone::CleanUp()
{
}

void ComponentBone::Enable()
{
	isActive = true;
}

void ComponentBone::Disable()
{
	isActive = false;
}

void ComponentBone::OnEditor()
{
}
