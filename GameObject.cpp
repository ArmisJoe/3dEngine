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

	if (aabb != nullptr) {
		glLineWidth(2.0f);
		glBegin(GL_LINES);
		for (uint i = 0; i < aabb->NumVertices; ++i)
		{
		}
		glVertex3f(0.f, 0.f, 0.f);
		glVertex3f(0.f, 10.f, 0.f);
		glEnd();
		glLineWidth(1.0f);

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
			CreateAABBFromMesh();

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

void GameObject::CreateAABBFromMesh()
{
	vector<Component*> meshes = FindComponents(componentType_Mesh);

	if (meshes.size() > 0) {

		uint num_vert = 0;
		vec *pointArray = nullptr;

		for (vector<Component*>::iterator it = meshes.begin(); it != meshes.end(); ++it)
		{
			ComponentMesh *curr_mesh = (ComponentMesh*)(*it);

			num_vert += curr_mesh->num_vertices;

			vec curr_it;

			for (uint i = 0; i < (curr_mesh->num_vertices * 3); ++i)
			{
				switch (i % 3) {
				case 0:

					if (i != 0)
						pointArray->Add(curr_it);
				
					curr_it.x = curr_mesh->vertices[i];
					break;
				case 1:
					curr_it.y = curr_mesh->vertices[i];
					break;
				case 2:
					curr_it.z = curr_mesh->vertices[i];
					break;
				}

			}

			pointArray->Add(curr_it);
		}

		AABB *aabb = new AABB();

		aabb->SetFrom(pointArray, num_vert);
		this->aabb = aabb;
	}

}

void GameObject::OnEditor()
{
	if (!components.empty()) {
		for (std::vector<Component*>::iterator it = components.begin(); it != components.end(); it++) {
			if((*it) != nullptr)
				(*it)->OnEditor();
		}
	}
	if (!children.empty()) {
		for (std::vector<GameObject*>::iterator it = children.begin(); it != children.end(); it++) {
			if((*it) != nullptr)
				(*it)->OnEditor();
		}
	}
}