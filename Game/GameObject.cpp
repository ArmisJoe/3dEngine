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
#include "ModuleParson.h"

GameObject::GameObject(GameObject * p) : parent(p)
{
	name = "GameObject";
	//this->AddComponent(componentType_Transform);
	UID = RandomNumber();
}

GameObject::GameObject()
{
	name = "GameObject";
    //this->AddComponent(componentType_Transform);
	UID = RandomNumber();
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
			/*if ((*it) != nullptr) {
				(*it)->CleanUp();
				//mdelete (*it); /// -> [EXAMINATE] TRIGGERS AN EXCEPTION (posible deleting what is already deleted [but it shouldn't])*/
			}
		components.clear();
	}

	// Let's do it like this for now.
	// v	v	v	v	v	v	v	
	if (!children.empty()) {
		for (std::vector<GameObject*>::iterator it = children.begin(); it != children.end(); it++) {
			if ((*it) != nullptr) {
				(*it)->CleanUp(); 
				//mdelete (*it); // We have children inside children now R�P --> are we deleting something already deleted??
			}
		}
		children.clear();
	}

	/*if (!aabbs.empty()) {
		for (std::vector<AABB*>::iterator it = aabbs.begin(); it != aabbs.end(); it++) {
			if ((*it) != nullptr) {
				delete[](*it);
			}
		}
		children.clear();
	}*/

}

void GameObject::SetRoot(bool root)
{
	isRoot = root;
}

bool GameObject::IsRoot() const
{
	return isRoot;
}

void GameObject::SetStatic(bool set)
{
	isStatic = set;
}

bool GameObject::IsStatic() const
{
	return isStatic;
}

std::vector<Component*> GameObject::FindComponents(const int type)
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

Component * GameObject::AddComponent(const int type, Component * componentPointer, bool fromReference)
{
	Component* newComponent = nullptr;

	if (!fromReference && componentPointer!= nullptr)
	{
		componentPointer->parent = this;
		components.push_back(componentPointer);
		componentPointer->Start();
	}
	else {
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
	}

	return newComponent;
}

void GameObject::DeleteChild(GameObject * go)
{
	for (int i = 0; i < children.size(); i++) {
		if (go == children[i]) {
			children[i]->CleanUp();
			mdelete[] children[i];
		}
	}
}

void GameObject::DestroyComponent(Component * componentPointer)
{
	if (!components.empty()) {
		for (std::vector<Component*>::iterator it = components.begin(); it != components.end(); it++) {
			if ((*it) == componentPointer && (*it) != nullptr) {
				(*it)->CleanUp();
				mdelete (*it);
				components.erase(it);
				break;
			}
		}
	}
}

void GameObject::SetParent(GameObject * p)
{

	parent = p;

	//if (p == nullptr) {
	//	LOG("ERROR Setting parent to '%s' gameobject. [nullptr parent]", this->name);
	//	return;
	//}
	//
	//if (parent != nullptr) {
	//	for (int i = 0; i < parent->children.size(); i++) {
	//		if (parent->children[i] == this) {
	//			parent->children.erase(parent->children.begin() + i);
	//			break;
	//		}
	//	}
	//	parent = nullptr;
	//}
	//
	//parent = p;
	//p->children.push_back(this);
}


void GameObject::CreateAABBFromMesh(ComponentMesh* mesh)
{
	if (mesh != nullptr) {
	//	std::vector<Component*> nana = GetParent()->FindComponents(componentType_Mesh);

		uint num_vert = 0;
		float3 pointArray = float3::zero;

		AABB tmpAABB;

		tmpAABB.SetNegativeInfinity();
		tmpAABB.Enclose((float3*)mesh->vertices, mesh->num_vertices);
		if (GetParent() != nullptr) {
			std::vector<Component*> cmp_tr = FindComponents(componentType_Transform);
			if (cmp_tr.size() > 0 && cmp_tr[0] != nullptr)
			{
				ComponentTransform* tmp = (ComponentTransform*)cmp_tr.back();
				OBB obb = tmpAABB.Transform(tmp->GetTransformMatrix());
				tmpAABB = obb.MinimalEnclosingAABB();
				aabb = tmpAABB;
			}
		}
	}
}

void GameObject::UpdateAABBFromMesh(ComponentMesh * mesh)
{
	if (mesh != nullptr) {

		aabb.SetNegativeInfinity();
		aabb.Enclose((float3*)mesh->vertices, mesh->num_vertices);
		aabb.TransformAsAABB(GetTransform()->GetTransformMatrix());
		/*		// VERSION 1
			aabb.SetNegativeInfinity();
			aabb.Enclose((float3*)mesh->vertices, mesh->num_vertices);
			math::OBB obb = aabb.Transform(GetTransform()->GetWorldMatrix());
			aabb = obb.MinimalEnclosingAABB();*/


			/*	//VERSION 2
				float4x4 matrix = GetTransform()->GetWorldMatrix();
				AABB transformed_bounding_box = aabb;
				transformed_bounding_box.TransformAsAABB(matrix);
				aabb = transformed_bounding_box;*/

				/*
				glPushMatrix();
				glMultMatrixf(GetTransform()->GetWorldMatrix().Transposed().ptr());
				glBindBuffer(GL_ARRAY_BUFFER, mesh->id_vertices);
				glPopMatrix();
				glBufferData(GL_ARRAY_BUFFER, sizeof(float) *mesh->num_vertices * 3, mesh->vertices, GL_STATIC_DRAW);*/

				/*// VERSION 3
				OBB obb = aabb.Transform(GetTransform()->GetWorldMatrix());
				aabb.SetFrom(obb);*/
	}
}


void GameObject::SetToGlobalTransform()
{
	/*
	if (!children.empty()) {
		for (int i = 0; i < children.size(); i++) {
				ComponentTransform* cTrans = children[i]->GetTransform(); 
				if (cTrans != nullptr && GetTransform() != nullptr) {
					cTrans->GlobalPosition = GetTransform()->position + cTrans->position;
					cTrans->GlobalRotation = GetTransform()->rotation * cTrans->rotation;
					cTrans->GlobalScale = GetTransform()->scale + cTrans->scale;
				}
				children[i]->SetToGlobalTransform();
		}
	}*/
}

ComponentTransform * GameObject::GetTransform()
{
	ComponentTransform* ret = nullptr;
	if (components.empty() == false) {
		std::vector<Component*> cts = FindComponents(componentType_Transform);
		if (!cts.empty()) {
			ret = (ComponentTransform*)cts[0];
		}
	}
	return ret;
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

void GameObject::OnHierarchyTree(bool skip_root)
{

	uint flags = 0;

	if(skip_root == true) {
		if (!children.empty()) {
			for (std::vector<GameObject*>::iterator it = children.begin(); it != children.end(); it++) {
				if ((*it) != nullptr)
					(*it)->OnHierarchyTree(false);
			}
		}
	}
	else {

		if (children.empty()) {
			flags |= ImGuiTreeNodeFlags_Leaf;
		}
		if (selected == true) {
			flags |= ImGuiTreeNodeFlags_Selected;
		}

		if (ImGui::TreeNodeEx(name.c_str(), flags)) {
			if (ImGui::IsItemClicked(0)) {
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

}

void GameObject::Serialize(JSON_Doc* doc) {
	if (doc == nullptr)
		return;

	doc->MoveToRootSection();

	doc->AddSectionArr("gameobjects");
	doc->MoveToSectionInsideArr("gameobjects", doc->GetArraySize("gameobjects") - 1);

	if (this == App->scene->GetRoot())
		doc->SetBool("isRoot", true);
	else
		doc->SetBool("isRoot", false);

	doc->SetBool("static", this->isStatic);

	// UIDs
	doc->SetNumber("uid", UID);
	if (parent != nullptr)
		doc->SetNumber("parentUID", parent->UID);
	else
		doc->SetNumber("parentUID", -1);
	// Name
	doc->SetString("name", name.c_str());
	// Components
	for (int i = 0; i < components.size(); i++) {
		doc->MoveToRootSection();
		doc->AddSectionArr("components");
		doc->MoveToSectionInsideArr("components", doc->GetArraySize("components") - 1);
		components[i]->Serialize(doc);
	}


	// Children
	for (int i = 0; i < children.size(); i++) {
		children[i]->Serialize(doc);
	}

	doc->MoveToRootSection(); // Politeness pls
}


