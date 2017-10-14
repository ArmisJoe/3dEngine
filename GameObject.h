#pragma once
#ifndef __GAMEOBJECT_H__
#define __GAMEOBJECT_H__

#include "glmath.h"
#include <list>
#include "Globals.h"

struct Mesh;
struct Texture;
struct Transform;
class Component;

enum componentType {
	componentType_Mesh = 0,
	componentType_Texture,
	// Unknown ALLWAYS Last
	componentType_Unknown
};

struct Transform {
	Transform() {
		position = { 0, 0, 0 };
		rotation = { 0, 0, 0 };
	}
	~Transform() {};

	vec3 position;
	vec3 rotation;
};

class GameObject {
public:
	GameObject();
	~GameObject();

public:	// General Methods
	virtual bool Update(float dt);
	virtual void CleanUp();
public: // Specific Methods
	Component* AddComponent(componentType type, Component * componentPointer = nullptr); // Adds a new Component to the GameObject. Return nullptr on failure.
	std::list<Component*> GetComponents(componentType type); // Return nullptr on failure;
private:
	bool activeSelf = false;
public:
	// Properties
	char* name = nullptr;
	Transform transform;
	unsigned int layer = 0;
	char* tag = nullptr;

private:
	std::list<Component*> components;

};


class Component {
	friend GameObject;

public:
	GameObject* gameObject = nullptr;
	Transform transform;
	char* tag = nullptr;
	componentType type;
protected:
	virtual bool Update(float dt) { return true; };
	virtual void CleanUp() {};
};

struct Mesh : public Component {

	unsigned int id_vertices = 0; // id in VRAM
	unsigned int num_indices = 0;
	unsigned int* indices = nullptr;

	unsigned int id_indices = 0; // id in VRAM
	unsigned int num_vertices = 0;
	float* vertices = nullptr;

	Texture* tex = nullptr; // Mesh Texture
	unsigned int id_UV = 0; // id in VRAM
	float* textureCoords; // UV Coords
	unsigned int num_UV = 0;

	float material_index; // material ID
	unsigned int num_UVChannels = 0;

protected:
	bool Update(float dt);
private:
	void Draw();

};

struct Texture : public Component {
	unsigned int id = 0;
	float w = 0;
	float h = 0;

	int format = 0;
};


#endif // !__GAMEOBJECT_H__
