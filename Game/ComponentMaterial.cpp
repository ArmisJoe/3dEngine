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

void ComponentMaterial::Serialize(JSON_Doc * doc)
{
	//doc->AddSectionArr("components");
	//doc->MoveToSectionInsideArr("components", doc->GetArraySize("components") - 1);
	//
	//doc->SetNumber("type", type);
	//
	//doc->SetArray("texture_channels");
	//for (int i = 0; i < texType_Unknown; i++) {
	//	if (texture_Channels[i] == nullptr)
	//		continue;
	//	texture_Channels[i]->Serialize(doc, i);
	//}
}

void Texture::OnEditor()
{
	std::string str = "Size: " + std::to_string((int)w) + "x" + std::to_string((int)h);
	ImGui::Text(str.c_str());
	ImGui::Text("Path: ");
	ImGui::Text(path);
	ImGui::Image((ImTextureID)id, ImVec2(120, 120));
	ImGui::Separator();
}

void Texture::Serialize(JSON_Doc * doc, int channel)
{
	doc->AddSectionArr("texture_channels");
	doc->MoveToSectionInsideArr("texture_channels", doc->GetArraySize("texture_channels") - 1);
	doc->SetNumber("channel", channel);
	doc->SetString("path", this->path);
}
