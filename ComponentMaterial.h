#pragma once

#include "Component.h"


struct Texture {
	Texture() {};
	virtual ~Texture() {};
public:
	//void DrawInspectorPanel();
public:
	unsigned int id = 0;
	float w = 0;
	float h = 0;

	int format = 0;
};


struct ComponentMaterial : public Component {
	ComponentMaterial() { };
	virtual ~ComponentMaterial() {};
public:
	void DrawInspectorPanel() {};
public:
	std::list<Texture*> diffuse;

};
