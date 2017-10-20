#pragma once

#include "Component.h"

#include "glmath.h"

class ComponentTransform : public Component {
public:
	ComponentTransform();
	ComponentTransform(GameObject* argparent);
	ComponentTransform(componentType argtype, GameObject* argparent);
	~ComponentTransform() {};
public:
	vec3 position;
	vec4 rotation;
	vec3 scale;
};
