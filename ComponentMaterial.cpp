#include "ComponentMaterial.h"

#include "ImGui\imgui.h"
#include <string>

void Texture::DrawInspectorPanel()
{
		std::string str = "Size: " + std::to_string((int)this->w) + "x" + std::to_string((int)this->h);
		ImGui::Text(str.c_str());
		ImGui::Image((ImTextureID)this->id, ImVec2(120, 120));
}

ComponentMaterial::ComponentMaterial() : Component(componentType_Material)
{
	name = "Material";
	numMax = 1;
	for (int i = 0; i < texType_Unknown; i++) {
		texture_Channels[i] = nullptr;
	}
}

ComponentMaterial::ComponentMaterial(GameObject* argparent) : Component(componentType_Material, argparent)
{
	name = "Material";
	numMax = 1;
	for (int i = 0; i < texType_Unknown; i++) {
		texture_Channels[i] = nullptr;
	}
}

ComponentMaterial::ComponentMaterial(componentType argtype, GameObject * argparent) : Component(argtype, argparent)
{
	name = "Material";
	numMax = 1;
	for (int i = 0; i < texType_Unknown; i++) {
		texture_Channels[i] = nullptr;
	}
}

void ComponentMaterial::OnEditor()
{
}

bool ComponentMaterial::HasTextures()
{
	bool ret = false;
	
	for (int i = 0; i < texType_Unknown; i++) {
		if (texture_Channels[i] != nullptr) {
			ret = true;
			break;
		}
	}

	return ret;
}
