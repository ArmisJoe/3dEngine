#include "GameObject.h"
#include "Application.h"
#include "ImGui\imgui.h"
#include <string>

#include "Component.h"
#include "ComponentMaterial.h"
#include "ComponentMesh.h"
#include "ComponentTransform.h"


GameObject::GameObject()
{
	name = "name";
}

GameObject::~GameObject()
{
}

void GameObject::Update()
{
	if (!components.empty()) {
		for (std::vector<Component*>::iterator it = components.begin(); it != components.end(); it++) {
			if ((*it) != nullptr) {
				(*it)->Update();
			}
		}
	}
	if (!children.empty()) {
		for (std::vector<GameObject*>::iterator it = children.begin(); it != children.end(); it++) {
			if ((*it) != nullptr) {
				(*it)->Update();
			}
		}
	}
}

void GameObject::CleanUp()
{
	if (!components.empty()) {
		for (std::vector<Component*>::iterator it = components.begin(); it != components.end(); it++) {
			if ((*it) != nullptr) {
				(*it)->CleanUp(); // Triggers an exeption error (possible deleting what is already deleted)
				delete (*it);
			}
		}
		components.clear();
	}

	// Let's do it like this for now.
	// v	v	v	v	v	v	v	
	if (!children.empty()) {
		for (std::vector<GameObject*>::iterator it = children.begin(); it != children.end(); it++) {
			if ((*it) != nullptr) {
				(*it)->CleanUp(); 
				delete (*it);
			}
		}
		children.clear();
	}

}

Component * GameObject::FindComponent(componentType type)
{
	Component* ret = nullptr;
	if (!components.empty()) {
		for (std::vector<Component*>::iterator it = components.begin(); it != components.end(); it++) {
			if ((*it) != nullptr && ((*it)->GetType() == type)) {
				ret = (*it);
			}
		}
	}
	return ret;
}

Component * GameObject::AddComponent(const componentType type, Component * componentPointer)
{
	Component* newComponent = nullptr;

	if (componentPointer == nullptr) {
		switch (type) {
		case componentType_Mesh:
			newComponent = new ComponentMesh(type, this);
			components.push_back(newComponent);
			break;
		case componentType_Material:
			newComponent = new ComponentMaterial(type, this);
			components.push_back(newComponent);
			break;
		case componentType_Transform:
			newComponent = new ComponentTransform(type, this);
			components.push_back(newComponent);
			break;
		case componentType_Unknown:
			break;
		default:
			break;
		}

	}
	else {
		newComponent = componentPointer;
		components.push_back(newComponent);
	}

	return newComponent;
}

void GameObject::DestroyComponent(const componentType type, Component * componentPointer)
{
	vector<Component*>::iterator position = find(components.begin(), components.end(), componentPointer);
	if (position != components.end()) {// == myVector.end() means the element was not found
		(*position)->CleanUp();
		components.erase(position);
	}
}

/* //????????
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
}*/

/* // This does no longer go here
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
*/

