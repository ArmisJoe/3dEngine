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
}

ComponentMaterial::ComponentMaterial(GameObject* argparent) : Component(componentType_Material, argparent)
{
}

ComponentMaterial::ComponentMaterial(componentType argtype, GameObject * argparent) : Component(argtype, argparent)
{}
