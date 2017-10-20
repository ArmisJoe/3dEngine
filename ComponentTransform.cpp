#include "ComponentTransform.h"

ComponentTransform::ComponentTransform() : Component(componentType_Transform)
{
}

ComponentTransform::ComponentTransform(GameObject* argparent) : Component(componentType_Transform, argparent)
{
	scale = { 1, 1, 1 };
}

ComponentTransform::ComponentTransform(componentType argtype, GameObject * argparent) : Component(argtype, argparent)
{
}
