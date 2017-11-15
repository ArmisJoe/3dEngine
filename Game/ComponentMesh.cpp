#include "ComponentMesh.h"

#include "Application.h"

#include "ImGui\imgui.h"
#include <string>

#include "ComponentTransform.h"

#include "md5.h"

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
	ImGui::TextColored(COLOR_YELLOW, "Mesh:");
		std::string str = "Triangles: " + std::to_string(this->num_triangles);
		ImGui::Text(str.c_str());
		str = "Vertices: " + std::to_string(this->num_vertices);
		ImGui::Text(str.c_str());
		ImGui::SameLine();
		str = "Indices: " + std::to_string(this->num_indices);
		ImGui::Text(str.c_str());
		ImGui::Text(path.c_str());
}
void ComponentMesh::Serialize(JSON_Doc * doc)
{
	if (doc == NULL)
		return;

	char* buffer;
	App->fs->Load(path.c_str(), &buffer);

	std::string meshdoc = md5(buffer).c_str();

	doc->SetNumber("type", type);
	doc->SetNumber("parentUID", (parent != nullptr) ? parent->GetUID() : -1);
	doc->SetString("path", path.c_str());
	doc->SetString("rawpath", raw_path.c_str());
	doc->SetString("meshdoc", meshdoc.c_str());
}
;
