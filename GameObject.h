#pragma once
#ifndef __GAMEOBJECT_H__
#define __GAMEOBJECT_H__

#include "glmath.h"
#include <list>
#include "Globals.h"
#include "Component.h"

class Component;

class GameObject {
public:
	GameObject();
	virtual ~GameObject();
public:	// General Methods
	virtual bool Update(float dt);
	virtual void CleanUp();
public: // Specific Methods
	Component* AddComponent(componentType type, Component * componentPointer = nullptr); // Adds a new Component to the GameObject. Return nullptr on failure.
	std::list<Component*> GetComponents(componentType type);
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
	std::list<Component*> components;
};

#endif // !__GAMEOBJECT_H__
