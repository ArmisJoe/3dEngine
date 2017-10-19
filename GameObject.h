#pragma once
#ifndef __GAMEOBJECT_H__
#define __GAMEOBJECT_H__

#include "glmath.h"
#include <vector>
#include <string>
#include "Globals.h"
#include "Component.h"

#include "ComponentMesh.h"
#include "ComponentMaterial.h"
#include "ComponentTransform.h"

class Component;

/*
class GameObject {
public:
	GameObject();
	virtual ~GameObject();
public:	// General Methods
	virtual bool Update(float dt);
	virtual void CleanUp();
public: // Specific Methods
	Component* AddComponent(componentType type, Component * componentPointer = nullptr); // Adds a new Component to the GameObject. Return nullptr on failure.

	std::vector<Component*> GetComponents(componentType type);

	GameObject* GetParent() {
		return parent;
	};
	void DrawInspectorPanel();
private:
	bool activeSelf = false;
public:
	// Properties
	char* name = nullptr;
	unsigned int layer = 0;
	char* tag = nullptr;
	ComponentTransform transform;
public:
	std::vector<Component*> components;
private:
	GameObject* parent;
	std::vector<GameObject*> child;

};
*/

class GameObject {
public:
	GameObject();
	~GameObject();
private:
	GameObject* parent;
	string name;
public:
	vector<Component*> components;
	vector<GameObject*> children; // DELCIOSIOS
public:
	void Update();
	void CleanUp();
	Component* FindComponent(componentType type);
	Component* AddComponent(const componentType type, Component* componentPointer = nullptr);
	void DestroyComponent(const componentType type, Component* componentPointer = nullptr);
	GameObject* GetParent() const { return parent; }
	string GetName() const { return name; }

};

#endif // !__GAMEOBJECT_H__


