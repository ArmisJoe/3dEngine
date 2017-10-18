#pragma once

#include "GameObject.h"


class ComponentTransform;

enum componentType {
	componentType_Mesh = 0,
	componentType_Material,
	componentType_Transform,
	// Unknown ALLWAYS Last
	componentType_Unknown
};

/*
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
};*/


class Component {
protected:
	componentType type;
	bool isActive = true;
	GameObject* parent;

public:
	virtual componentType GetType() { return type; }
	virtual void Update() {}
	virtual void CleanUp() {}
	virtual void Enable() { isActive = true; }
	virtual void Disable() { isActive = false; }
	GameObject* GetParent() const { return parent; }
	bool SetParent(GameObject* parent);
	void SetType(componentType ntype) { type = ntype; }
};