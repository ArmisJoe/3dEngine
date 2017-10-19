#include "ComponentTransform.h"

ComponentTransform::ComponentTransform() : Component(componentType_Transform)
{
	scale = { 1, 1, 1 };
}

ComponentTransform::ComponentTransform(componentType argtype, GameObject * argparent) : Component(argtype, argparent)
{
}
