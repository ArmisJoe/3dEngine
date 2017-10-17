#pragma once

#include "GameObject.h"
#include "ComponentTransform.h"

class GameObject;

enum componentType {
	componentType_Mesh = 0,
	componentType_Texture,
	// Unknown ALLWAYS Last
	componentType_Unknown
};

class Component {
	friend GameObject;
public:
	Component() { name = "Component"; };
	virtual ~Component() {};
public:
	virtual void DrawInspectorPanel() {};
public:
	GameObject* gameObject = nullptr;
	ComponentTransform transform;
	char* tag = nullptr;
	componentType type;
	char* name = nullptr;
public:
	virtual bool Update(float dt) { return true; };
	virtual void CleanUp() {};
};
