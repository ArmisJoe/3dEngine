#include "ComponentMaterial.h"

#include "ImGui\imgui.h"
#include <string>

ComponentMaterial::ComponentMaterial() : Component(componentType_Material)
{
	name = "Material";
	for (int i = 0; i < texType_Unknown; i++) {
		texture_Channels[i] = nullptr;
	}
}

ComponentMaterial::ComponentMaterial(GameObject* argparent) : Component(componentType_Material, argparent)
{
	name = "Material";
	for (int i = 0; i < texType_Unknown; i++) {
		texture_Channels[i] = nullptr;
	}
}

ComponentMaterial::ComponentMaterial(componentType argtype, GameObject * argparent) : Component(argtype, argparent)
{
	name = "Material";
	for (int i = 0; i < texType_Unknown; i++) {
		texture_Channels[i] = nullptr;
	}
}

void ComponentMaterial::OnEditor()
{
	Texture* tex = nullptr;
	//Diffuse
	tex = texture_Channels[texType_Diffuse];
	if (tex != nullptr) {
		if (ImGui::CollapsingHeader("Diffuse: ")) {
			tex->OnEditor();
		}
	}
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

void Texture::OnEditor()
{
	std::string str = "Size: " + std::to_string((int)w) + "x" + std::to_string((int)h);
	ImGui::Text(str.c_str());
	//ImGui::Text("Path: ");
	//ImGui::Text(path);
	ImGui::Image((ImTextureID)id, ImVec2(120, 120));
	ImGui::Separator();
}
