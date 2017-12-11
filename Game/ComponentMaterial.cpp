#include "ComponentMaterial.h"

#include "ImGui\imgui.h"
#include <string>

ComponentMaterial::ComponentMaterial() : Component(componentType_Material)
{
	typeName = "Material";
	name = "Material";
	numMax = 1;
	for (int i = 0; i < texType_Unknown; i++) {
		texture_Channels[i] = nullptr;
	}
}

ComponentMaterial::ComponentMaterial(GameObject* argparent) : Component(componentType_Material, argparent)
{
	typeName = "Material";
	name = "Material";
	numMax = 1;
	for (int i = 0; i < texType_Unknown; i++) {
		texture_Channels[i] = nullptr;
	}
}

ComponentMaterial::ComponentMaterial(componentType argtype, GameObject * argparent) : Component(componentType_Material, argparent)
{
	typeName = "Material";
	name = "Material";
	numMax = 1;
	for (int i = 0; i < texType_Unknown; i++) {
		texture_Channels[i] = nullptr;
	}
}

void ComponentMaterial::OnEditor()
{
	Texture* tex = nullptr;
	ImGui::TextColored(COLOR_YELLOW, "Material:");
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
	// Type
	doc->SetNumber("type", type);
	doc->SetNumber("parentUID", (parent != nullptr) ? parent->GetUID() : -1);
	// Texture Channels
	doc->SetArray("texture_channels");
	for (int i = 0; i < texType_Unknown; i++) {
		if (texture_Channels[i] == nullptr)
			continue;
		doc->MoveToSection("gameobjects");
		doc->AddSectionArr("texture_channels");
		doc->MoveToSectionInsideArr("texture_channels", doc->GetArraySize("texture_channels") - 1);
		texture_Channels[i]->Serialize(doc, i);
	}
}

void Texture::OnEditor()
{
	std::string str = "Size: " + std::to_string((int)w) + "x" + std::to_string((int)h);
	ImGui::Text(str.c_str());
	ImGui::Text("Path: ");
	ImGui::Text(path.c_str());
	ImGui::Image((ImTextureID)id, ImVec2(120, 120));
	ImGui::Separator();
}

void Texture::Serialize(JSON_Doc * doc, int channel)
{
	doc->SetNumber("channel", channel);
	doc->SetString("path", this->path.c_str());
}
