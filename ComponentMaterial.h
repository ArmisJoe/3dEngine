#pragma once

#include "Component.h"


struct Texture : public Component {
	Texture() { name = "Texture"; };
	virtual ~Texture() {};
public:
	void DrawInspectorPanel();
public:
	unsigned int id = 0;
	float w = 0;
	float h = 0;

	int format = 0;
};


struct ComponentMaterial : public Component {
	ComponentMaterial() { name = "Material"; };
	virtual ~ComponentMaterial() {};
public:
	void DrawInspectorPanel() {};
public:
	std::list<Texture*> diffuse;

};
