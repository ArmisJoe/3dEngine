#pragma once
#ifndef __GAMEOBJECT_H__
#define __GAMEOBJECT_H__

#include "src\MathGeoLib.h"
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
	std::vector<AABB> aabbs;
public:
	void Start();
	void Update(float dt); // Called every ModuleScene->Update(dt) (if they are insied the scene)
	void CleanUp(); // Called at App CleanUp
public:
	std::vector<Component*> FindComponents(componentType type);
	Component* AddComponent(const componentType type, Component* componentPointer = nullptr, bool fromReference = true);
	void DeleteChild(GameObject* go);
	void DestroyComponent(Component* componentPointer = nullptr);
	GameObject* GetParent() const { return parent; }
	void SetParent(GameObject* p) { parent = p; }
	std::string GetName() const { return name; }
	void CreateAABBFromMesh(ComponentMesh* mesh);
	//AABB* GetAABB() const { return aabb; }

	void OnEditor();

};

#endif // !__GAMEOBJECT_H__


