#include "ComponentMaterial.h"

#include "ImGui\imgui.h"
#include <string>

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

ComponentMaterial::ComponentMaterial(componentType argtype, GameObject * argparent) : Component(componentType_Material, argparent)
{
	name = "Material";
	numMax = 1;
	for (int i = 0; i < texType_Unknown; i++) {
		texture_Channels[i] = nullptr;
	}
}

void ComponentMaterial::OnEditor()
{
	Texture* tex = nullptr;
	const char* tex_path = nullptr;
	//Diffuse
	//tex = texture_Channels[texType_Diffuse];
	//if (tex != nullptr) {
	//	tex_path = tex->path;
	//	ImGui::InputText("Diffuse: ", tex_path, sizeof(tex_path));
	//	std::string str = "Size: " + std::to_string((int)tex->w) + "x" + std::to_string((int)tex->h);
	//	ImGui::Text(str.c_str());
	//	ImGui::Image((ImTextureID)tex->id, ImVec2(120, 120));
	//	ImGui::Separator();
	//}
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
