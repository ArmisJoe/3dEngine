#pragma once

#include "Component.h"
#include <list>

struct Texture {
	Texture() {};
	virtual ~Texture() {};
public:
	void OnEditor();
public:
	unsigned int id = 0;
	float w = 0;
	float h = 0;

	const char* path = nullptr;

	int format = 0;
};

enum texType {
	texType_Diffuse = 0,
	//Unknown Always Last
	texType_Unknown
};

class ComponentMaterial : public Component {
public:
	ComponentMaterial();
	ComponentMaterial(GameObject* argparent);
	ComponentMaterial(componentType argtype, GameObject* argparent);
	virtual ~ComponentMaterial() {};
public:
	virtual void OnEditor();
	bool HasTextures();
	Texture* GetTextureChannel(const int texType_Channel) const {
		return texture_Channels[texType_Channel];
	}
	void SetTextureChannel(const int texType_Channel, Texture* tex) {
		if (tex != nullptr) {
			if (texType_Channel < texType_Unknown) {
				texture_Channels[texType_Channel] = tex;
			}
		}
	}
private:
	Texture* texture_Channels[texType_Unknown];

};
