#pragma once
#ifndef __GAMEOBJECT_H__
#define __GAMEOBJECT_H__

#include "MathGeoLib\MathGeoLib.h"
#include "glmath.h"
#include <vector>
#include <string>
#include "Globals.h"
#include "ModuleParson.h"

class Component;

class ComponentMesh;
class ComponentMaterial;
class ComponentTransform;

class ModuleScene;

enum componentType;

class GameObject {
public:
	GameObject(GameObject* parent);
	GameObject();
	virtual ~GameObject();
private:
	GameObject* parent = nullptr;
	std::string name;

public:
	std::vector<Component*> components; // Children Components
	std::vector<GameObject*> children; // Children GameObjects
	AABB aabb;

	bool selected = false;

private:
	ModuleScene* scene = nullptr;
	bool isRoot = false;
	bool isStatic = true;
public:
	void Start();
	void Update(float dt); // Called every ModuleScene->Update(dt) (if they are insied the scene)
	void CleanUp(); // Called at App CleanUp

public:
	void SetRoot(bool root);
	bool IsRoot() const;
	void SetStatic(bool set);
	bool IsStatic() const;

	double GetUID() const {
		return UID;
	}
	void SetUID(double uid) {
		UID = uid;
	}
	double GetParentUID() const {
		return parentUID;
	}
	void SetParentUID(double puid) {
		parentUID = puid;
	}

	void Serialize(JSON_Doc* doc);


	std::vector<Component*> FindComponents(const int type);
	Component* AddComponent(const int type, Component* componentPointer = nullptr, bool fromReference = true);
	void DestroyComponent(Component* componentPointer = nullptr);
	GameObject* GetParent() const { return parent; }
	void SetParent(GameObject* p);
	std::string GetName() const { return name; }
	void CreateAABBFromMesh(ComponentMesh* mesh);
	void UpdateAABBFromMesh(ComponentMesh* mesh);
	void SetName(const char* str) { name = str; }
	void SetToGlobalTransform();
	ComponentTransform* GetTransform();

	void SetScene(ModuleScene* sce);

	void OnEditor();
	void OnHierarchyTree(bool skip_root = false);

	void WantToRemoveThis() {
		to_remove = true;
	}
	void WantToRemoveChild(GameObject* child) {
		if (child == nullptr)
			return;
		for (int i = 0; i < children.size(); i++) {
			if (children[i] == child)
				children[i]->WantToRemoveThis();
		}
	}
	void RemoveIteration(bool toSelf = true);

private:
	bool to_remove = false;
	void RemoveThis();
	void RemoveChild(GameObject* child);

	void RemoveComponent(Component* c);

private: // Serialization Values
	double UID;
	double parentUID;

};

#endif // !__GAMEOBJECT_H__


