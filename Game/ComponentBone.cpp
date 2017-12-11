#include "ComponentBone.h"

ComponentBone::ComponentBone() : Component(componentType_Bone)
{
	typeName = "Skeleton";
	name = "Bone";
}

ComponentBone::ComponentBone(GameObject * argparent) : Component(componentType_Bone, argparent)
{
	typeName = "Skeleton";
	name = "Bone";
}
 
ComponentBone::ComponentBone(componentType argtype, GameObject * argparent) : Component(componentType_Bone, argparent)
{
	typeName = "Skeleton";
	name = "Bone";
}

ComponentBone::~ComponentBone()
{
	typeName = "Skeleton";
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

ComponentMesh * ComponentBone::GetMesh() const
{
	return mesh;
}

void ComponentBone::SetMesh(ComponentMesh * m)
{
	mesh = m;
}
