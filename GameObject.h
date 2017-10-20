#pragma once
#ifndef __GAMEOBJECT_H__
#define __GAMEOBJECT_H__

#include "glmath.h"
#include <vector>
#include <string>
#include "Globals.h"

class Component;

class ComponentMesh;
class ComponentMaterial;
class ComponentTransform;

enum componentType;

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
	GameObject(GameObject* parent);
	GameObject();
	virtual ~GameObject();
private:
	GameObject* parent;
	std::string name;
public:
	std::vector<Component*> components;
	std::vector<GameObject*> children; // DELCIOSIOS
public:
	void Update(float dt);
	void CleanUp();
public:
	std::vector<Component*> FindComponents(componentType type);
	Component* AddComponent(const componentType type, Component* componentPointer = nullptr);
	void DestroyComponent(Component* componentPointer = nullptr);
	GameObject* GetParent() const { return parent; }
	void SetParent(GameObject* p) { parent = p; }
	std::string GetName() const { return name; }

	void OnEditor();

};

#endif // !__GAMEOBJECT_H__


