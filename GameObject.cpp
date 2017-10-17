#include "GameObject.h"
#include "Application.h"
#include "ImGui\imgui.h"
#include <string>
GameObject::GameObject()
{
	name = "name";
}

GameObject::~GameObject()
{
}

bool GameObject::Update(float dt)
{
	if (!components.empty()) {
		for (std::vector<Component*>::iterator it = components.begin(); it != components.end(); it++) {
			if ((*it) != nullptr) {
				//(*it)->Update(dt);
			}
		}
	}

	return true;
}

void GameObject::CleanUp()
{
	if (!components.empty()) {
		for (std::vector<Component*>::iterator it = components.begin(); it != components.end(); it++) {
			if ((*it) != nullptr) {
				//(*it)->CleanUp(); // Triggers an exeption error (possible deleting what is already deleted)
				//delete (*it);
			}
		}
		components.clear();
	}
}

Component * GameObject::AddComponent(componentType type, Component * componentPointer)
{
	Component* newComponent = nullptr;

	if (componentPointer == nullptr) {
		switch (type) {
		case componentType_Mesh:
			newComponent = new Mesh();
			break;
		case componentType_Texture:
			newComponent = new Texture();
			break;
		case componentType_Unknown:
			break;
		}
	}
	else {
		newComponent = componentPointer;
	}

	if (newComponent != nullptr) {
		newComponent->type = type;
		newComponent->gameObject = this;
		newComponent->transform = this->transform;
		newComponent->tag = this->tag;

		components.push_back(newComponent);
	}

	return newComponent;
}

std::vector<Component*> GameObject::GetComponents(componentType type)
{
	std::vector<Component*> cs;

	for (std::vector<Component*>::iterator it = components.begin(); it != components.end(); it++) {
		if ((*it) != nullptr) {
			if ((*it)->type == type)
				cs.push_back((*it));
		}
	}

	return cs;
}
void GameObject::DrawInspectorPanel()
{
	if (ImGui::CollapsingHeader(this->name, false)) {
		if (ImGui::CollapsingHeader("Transform", true)) {
			//Position
			std::string str = "X: " + std::to_string(this->transform.position.x);
			ImGui::Text("Position: ");
			ImGui::Text(str.c_str());
			ImGui::SameLine();
			str = "Y: " + std::to_string(this->transform.position.y);
			ImGui::Text(str.c_str());
			ImGui::SameLine();
			str = "Z: " + std::to_string(this->transform.position.z);
			ImGui::Text(str.c_str());
			//Rotation
			ImGui::Text("Rotation: ");
			str = "X: " + std::to_string(this->transform.rotation.x);
			ImGui::Text(str.c_str());
			ImGui::SameLine();
			str = "Y: " + std::to_string(this->transform.rotation.y);
			ImGui::Text(str.c_str());
			ImGui::SameLine();
			str = "Z: " + std::to_string(this->transform.rotation.z);
			ImGui::Text(str.c_str());
		}
		for (std::vector<Component*>::iterator c_it = this->components.begin(); c_it != this->components.end(); c_it++) {
			Component* c = (*c_it);
			if (ImGui::CollapsingHeader(c->name, true)) {
				c->DrawInspectorPanel();
			}
		}
	}
};

void Mesh::DrawInspectorPanel()
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

void Texture::DrawInspectorPanel()
{
	std::string str = "Size: " + std::to_string((int)this->w) + "x"+ std::to_string((int)this->h);
	ImGui::Text(str.c_str());
	ImGui::Image((ImTextureID)this->id, ImVec2(120, 120));
}
