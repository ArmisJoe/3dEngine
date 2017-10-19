#include "ComponentMesh.h"

#include "Application.h"

#include "ImGui\imgui.h"
#include <string>

ComponentMesh::ComponentMesh() : Component(componentType_Mesh)
{
}

ComponentMesh::ComponentMesh(componentType argtype, GameObject * argparent) : Component(argtype, argparent)
{
}

void ComponentMesh::Update(float dt)
{
	if (visible) {
		ComponentMaterial* mat = (ComponentMaterial*)this->GetParent()->FindComponents(componentType_Material)[0];
		application->renderer3D->DrawMesh(this, mat);
	}

}

void ComponentMesh::DrawInspectorPanel()
{
		std::string str = "Triangles: " + std::to_string(this->num_triangles);
		ImGui::Text(str.c_str());
		str = "Vertices: " + std::to_string(this->num_vertices);
		ImGui::Text(str.c_str());
		ImGui::SameLine();
		str = "Indices: " + std::to_string(this->num_indices);
		ImGui::Text(str.c_str());
		ImGui::Text("Scale:");
		str = "X: " + std::to_string(this->transform.scale.x);
		ImGui::Text(str.c_str());
		ImGui::SameLine();
		str = "Y: " + std::to_string(this->transform.scale.y);
		ImGui::Text(str.c_str());
		ImGui::SameLine();
		str = "Z: " + std::to_string(this->transform.scale.z);
		ImGui::Text(str.c_str());
}
