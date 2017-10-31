#include "ComponentCamera.h"

ComponentCamera::ComponentCamera() : Component(componentType_Camera)
{
	name = "Camera";
}

ComponentCamera::ComponentCamera(GameObject * argparent) : Component(componentType_Camera)
{
	name = "Camera";
}

ComponentCamera::ComponentCamera(componentType argtype, GameObject * argparent) : Component(componentType_Camera)
{
	name = "Camera";
}

void ComponentCamera::OnEditor()
{
}

void ComponentCamera::Start()
{
}

void ComponentCamera::Update()
{

}

void ComponentCamera::CleanUp()
{
}
