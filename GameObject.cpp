#include "GameObject.h"
#include "Application.h"
#include "ImGui\imgui.h"
#include <string>
#include <assert.h>

#include "Component.h"
#include "ComponentMaterial.h"
#include "ComponentMesh.h"
#include "ComponentTransform.h"

#include "PanelInspector.h"


GameObject::GameObject(GameObject * p) : parent(p)
{
	name = "GameObject";
	this->AddComponent(componentType_Transform);
}

GameObject::GameObject()
{
	name = "GameObject";
	this->AddComponent(componentType_Transform);
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
				//delete[] (*it); /// -> [EXAMINATE] TRIGGERS AN EXCEPTION (posible deleting what is already deleted [but it shouldn't])
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
			if (this->FindComponents(type).empty())
				newComponent = new ComponentMaterial(this);
			break;
		case componentType_Transform:
			if (this->FindComponents(type).empty())
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

void GameObject::DeleteChild(GameObject * go)
{
	for (int i = 0; i < children.size(); i++) {
		if (go == children[i]) {
			children[i]->CleanUp();
			delete[] children[i];
		}
	}
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

void GameObject::SetScene(ModuleScene * sce)
{
	scene = sce;
}

void GameObject::OnEditor()
{
	if (!components.empty()) {
		for (std::vector<Component*>::iterator it = components.begin(); it != components.end(); it++) {
			if((*it) != nullptr)
				(*it)->OnEditor();
		}
	}
}

void GameObject::OnHierarchyTree()
{

	uint flags = 0;

	if (children.empty()) {
		flags |= ImGuiTreeNodeFlags_Leaf;
	}
	if (selected == true) {
		flags |= ImGuiTreeNodeFlags_Selected;
	}
	if (ImGui::TreeNodeEx(name.c_str(), flags)) {
		if (ImGui::IsItemClicked(1)) {
			if (scene != nullptr) {
				scene->SetSelected(this);
			}
		}
		if (!children.empty()) {
			for (std::vector<GameObject*>::iterator it = children.begin(); it != children.end(); it++) {
				if ((*it) != nullptr)
					(*it)->OnHierarchyTree();
			}
		}
		ImGui::TreePop();
	}

}
