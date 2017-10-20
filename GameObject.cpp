#include "GameObject.h"
#include "Application.h"
#include "ImGui\imgui.h"
#include <string>
#include <assert.h>

#include "Component.h"
#include "ComponentMaterial.h"
#include "ComponentMesh.h"
#include "ComponentTransform.h"


GameObject::GameObject(GameObject * p) : parent(p)
{
}

GameObject::GameObject()
{
	name = "name";
}

GameObject::~GameObject()
{
}

void GameObject::Update(float dt)
{
	if (!components.empty()) {
		for (std::vector<Component*>::iterator it = components.begin(); it != components.end(); it++) {
			if ((*it) != nullptr) {
				(*it)->Update(dt);
			}
		}
	}
	if (!children.empty()) {
		for (std::vector<GameObject*>::iterator it = children.begin(); it != children.end(); it++) {
			if ((*it) != nullptr) {
				(*it)->Update(dt);
			}
		}
	}
}

void GameObject::CleanUp()
{
	if (!components.empty()) {
		for (std::vector<Component*>::iterator it = components.begin(); it != components.end(); it++) {
			if ((*it) != nullptr) {
				(*it)->CleanUp();
				delete[](*it);
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
				delete[] (*it);
			}
		}
		children.clear();
	}

}

std::vector<Component*> GameObject::FindComponents(componentType type)
{
	std::vector<Component*> ret;
	if (!components.empty()) {
		for (std::vector<Component*>::iterator it = components.begin(); it != components.end(); it++) {
			if ((*it) != nullptr && ((*it)->GetType() == type)) {
				ret.push_back((*it));
			}
		}
	}
	return ret;
}

Component * GameObject::AddComponent(const componentType type, Component * componentPointer)
{
	Component* newComponent = nullptr;

	// Create the New Component
	switch (type) {
		case componentType_Mesh:
			newComponent = new ComponentMesh(this);
			break;
		case componentType_Material:
			newComponent = new ComponentMaterial(this);
			break;
		case componentType_Transform:
			newComponent = new ComponentTransform(this);
			break;
	}

	// Copy the 'Reference' Component into the New Component
	if (componentPointer != nullptr && newComponent != nullptr) {
		assert(componentPointer->type == newComponent->type);

		uint bufferSize = 0;

		switch (newComponent->type) {
		case componentType_Mesh:
			bufferSize = sizeof(ComponentMesh);
			break;
		case componentType_Material:
			bufferSize = sizeof(ComponentMaterial);
			break;
		case componentType_Transform:
			bufferSize = sizeof(ComponentTransform);
			break;
		}

		if (bufferSize != 0) {
			memcpy(newComponent, componentPointer, bufferSize);
		}
	}

	if (newComponent != nullptr) {
		newComponent->parent = this;
		components.push_back(newComponent);
	}
	
	return newComponent;
}

void GameObject::DestroyComponent(Component * componentPointer)
{
	if (!components.empty()) {
		for (std::vector<Component*>::iterator it = components.begin(); it != components.end(); it++) {
			if ((*it) == componentPointer && (*it) != nullptr) {
				(*it)->CleanUp();
				components.erase(it);
				delete[](*it);
			}
		}
	}
}

void GameObject::OnEditor()
{
	if (!components.empty()) {
		for (std::vector<Component*>::iterator it = components.begin(); it != components.end(); it++) {
			(*it)->OnEditor();
		}
	}
	if (!children.empty()) {
		for (std::vector<GameObject*>::iterator it = children.begin(); it != children.end(); it++) {
			(*it)->OnEditor();
		}
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

