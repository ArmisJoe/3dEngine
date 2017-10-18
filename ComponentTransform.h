#pragma once

#include "Component.h"

#include "glmath.h"

class ComponentTransform : public Component {
public:
	ComponentTransform(componentType argtype, GameObject* argparent);
	~ComponentTransform() {};

	vec3 position;
	vec4 rotation;
	vec3 size;
};
