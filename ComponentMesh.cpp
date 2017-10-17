#include "ComponentMesh.h"

#include "ImGui\imgui.h"
#include <string>


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
		str = "X: " + std::to_string(this->scale.x);
		ImGui::Text(str.c_str());
		ImGui::SameLine();
		str = "Y: " + std::to_string(this->scale.y);
		ImGui::Text(str.c_str());
		ImGui::SameLine();
		str = "Z: " + std::to_string(this->scale.z);
		ImGui::Text(str.c_str());
}
