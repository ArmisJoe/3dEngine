#pragma once

#include "GameObject.h"

#include <string>

class Application;
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
	friend class GameObject;
public:
	Component(const componentType argtype);
	Component(componentType argtype, GameObject* argparent);
private:
	bool SetParent(GameObject* parent);
public:
	std::string name;
	componentType type;
protected:
	bool isActive = true;
	GameObject* parent = nullptr;
	uint numMax = NULL;
public:
	virtual componentType GetType() const { return type; }
	virtual void Update(float dt) {}
	virtual void CleanUp() {}

	virtual void Enable() { isActive = true; }
	virtual void Disable() { isActive = false; }
	virtual void OnEditor() {}

	GameObject* GetParent() const { return parent; }
	bool HasParent() const { return parent != nullptr; }
	void SetType(componentType ntype) { type = ntype; }
};