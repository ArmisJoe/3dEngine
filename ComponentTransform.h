#pragma once

#include "Component.h"

#include "glmath.h"
#include "src\MathGeoLib.h"

class ComponentTransform : public Component {
public:
	ComponentTransform();
	ComponentTransform(GameObject* argparent);
	ComponentTransform(componentType argtype, GameObject* argparent);
	~ComponentTransform() {};
public:
	void OnEditor();
public:
	float3 position;
	Quat rotation;
	float3 scale;
};
