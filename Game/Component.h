#pragma once

#include "GameObject.h"

#include "ModuleParson.h"
#include "Globals.h"

#include <string>


class ComponentTransform;

enum componentType {
	componentType_Mesh = 0,
	componentType_Material,
	componentType_Transform,
	componentType_Camera,
	componentType_Animation,
	componentType_Bone,
	// Unknown ALWAYS Last
	componentType_Unknown
};


class Component {
	friend class GameObject;
public:
	Component(const componentType argtype);
	Component(componentType argtype, GameObject* argparent);
protected:
	const char* typeName = "Component";
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
	virtual void Start() {}
	virtual void Update(float dt) {}
	virtual void CleanUp() {}

	virtual void Enable() { isActive = true; }
	virtual void Disable() { isActive = false; }
	virtual void OnEditor() {}

	GameObject* GetParent() const { return parent; }
	bool HasParent() const { return parent != nullptr; }
	void SetType(componentType ntype) { type = ntype; }
	double GetGoUID() const {
		return gameObjectUID;
	}
	void SetGoUID(const double uid) {
		gameObjectUID = uid;
	}

	virtual void Serialize(JSON_Doc* doc);

	void RemoveThis(); // NOT WORKING

private: // Serialize Values
	double UID;
	double gameObjectUID;

};