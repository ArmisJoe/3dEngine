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

void GameObject::Start()
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

	if (!aabbs.empty()) {
		for (std::vector<AABB*>::iterator it = aabbs.begin(); it != aabbs.end(); it++) {
			if ((*it) != nullptr) {
				delete[](*it);
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
	case componentType_Camera:
		if (this->FindComponents(type).empty())
			newComponent = new ComponentCamera(this);
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
		case componentType_Camera:
			bufferSize = sizeof(ComponentCamera);
			break;
		}

		if (bufferSize != 0) {
			memcpy(newComponent, componentPointer, bufferSize);
		}
	}

	if (newComponent != nullptr) {
		newComponent->parent = this;
		components.push_back(newComponent);
		newComponent->Start();
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
		float3 pointArray = float3::zero;

		for (vector<Component*>::iterator it = meshes.begin(); it != meshes.end(); ++it)
		{
			ComponentMesh *curr_mesh = (ComponentMesh*)(*it);

			AABB *aabb = new AABB();

			aabb->SetNegativeInfinity();
			aabb->Enclose((float3*)curr_mesh->vertices, curr_mesh->num_vertices);
			if (GetParent() != nullptr) {
				ComponentTransform * cmp_tr = (ComponentTransform*)GetParent()->FindComponents(componentType_Transform)[0];
				OBB tmp = aabb->Transform(cmp_tr->GetWorldMatrix());
				aabb = &tmp.MinimalEnclosingAABB();

				this->aabbs.push_back(aabb);
			}
			/*aabb->
			GetMesh()->box.SetNegativeInfinity();
			GetMesh()->box.Enclose((float3*)GetMesh()->vertices, GetMesh()->num_vertices);
			math::OBB obb = GetMesh()->box.Transform(GetGameObject()->GetGlobalTransfomMatrix());
			GetMesh()->box = obb.MinimalEnclosingAABB();*/


		/*	num_vert += curr_mesh->num_vertices;

			float3 curr_it;
			uint c = 0;
			for (uint i = curr_mesh->num_vertices * 3; i > 0; i--)
			{
				switch (i % 3) {
				case 0:

					if (c != 0)
						pointArray.Add(curr_it);
				
					curr_it.x = curr_mesh->vertices[c];
					break;
				case 1:
					curr_it.y = curr_mesh->vertices[c];
					break;
				case 2:
					curr_it.z = curr_mesh->vertices[c];
					break;
				}
				c++;
			}
			pointArray.Add(curr_it);*/
		}

		

		//aabb->SetFrom(&pointArray, num_vert);
		//this->aabb = aabb;
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