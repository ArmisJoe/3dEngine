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

class GameObject {
public:
	GameObject(GameObject* parent);
	GameObject();
	virtual ~GameObject();
private:
	GameObject* parent;
	std::string name;
public:
	std::vector<Component*> components; // Children Components
	std::vector<GameObject*> children; // Children GameObjects
public:
	void Update(float dt); // Called every ModuleScene->Update(dt) (if they are insied the scene)
	void CleanUp(); // Called at App CleanUp
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


