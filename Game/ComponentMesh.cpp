#include "ComponentMesh.h"

#include "Application.h"

#include "ImGui\imgui.h"
#include <string>

#include "ComponentTransform.h"

ComponentMesh::ComponentMesh() : Component(componentType_Mesh)
{
	name = "Mesh";
}

ComponentMesh::ComponentMesh(GameObject* argparent) : Component(componentType_Mesh, argparent)
{
	name = "Mesh";
}

ComponentMesh::ComponentMesh(componentType argtype, GameObject * argparent) : Component(componentType_Mesh, argparent)
{
	name = "Mesh";
}

void ComponentMesh::Start()
{
	GetParent()->CreateAABBFromMesh(this);
}

void ComponentMesh::Update(float dt)
{
	/**/
	if (visible) {
		ComponentMaterial* mat = nullptr;
		if (!this->GetParent()->FindComponents(componentType_Material).empty()) {
			mat = (ComponentMaterial*)this->GetParent()->FindComponents(componentType_Material)[0];
		}
	}
}

void ComponentMesh::Draw(const ComponentTransform* trans, const ComponentMaterial * mat)
{
}

void ComponentMesh::OnEditor()
{
	std::string str = "Triangles: " + std::to_string(this->num_triangles);
	ImGui::Text(str.c_str());
	str = "Vertices: " + std::to_string(this->num_vertices);
	ImGui::Text(str.c_str());
	ImGui::SameLine();
	str = "Indices: " + std::to_string(this->num_indices);
	ImGui::Text(str.c_str());
}
void ComponentMesh::Serialize(JSON_Doc * doc)
{
	if (doc == nullptr)
		return;

	doc->SetNumber("type", type);
	doc->SetString("dummy", "test");
	//doc->SetString("path", path);

}
;
