#include "GameObject.h"
#include "Application.h"
#include "ImGui\imgui.h"
#include <string>
#include <assert.h>

#include "Component.h"
#include "ComponentMaterial.h"
#include "ComponentMesh.h"
#include "ComponentTransform.h"
#include "ComponentAnimation.h"
#include "ComponentBone.h"

#include "PanelInspector.h"
#include "ModuleParson.h"

GameObject::GameObject(GameObject * p) : parent(p)
{
	name = "GameObject";
	this->AddComponent(componentType_Transform);
	UID = RandomNumber();
}

GameObject::GameObject()
{
	name = "GameObject";
    this->AddComponent(componentType_Transform);
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

	if (GetTransform() != nullptr)
	{
		if (GetTransform()->transform_modified == true)
		{
			OnUpdateTransform();
			AABBneedsUpdate = true;
		}
	}


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
			if ((*it) != nullptr && ((*it)->type == componentType_Transform || (*it)->type == componentType_Camera)) {	// Transform is the only component not passed by reference
				(*it)->CleanUp();
				if((*it) != nullptr)
					mdelete(*it);
				(*it) = nullptr;
			}
		}
		components.clear();
	}

	if (!children.empty()) {
		for (std::vector<GameObject*>::iterator it = children.begin(); it != children.end(); it++) {
			if ((*it) != nullptr) {
				(*it)->CleanUp();
			}
		}
		children.clear();
	}

	App->quadTree->RemoveObject(this);

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

void GameObject::SwitchStatic(bool set)
{
	if (set == true)
		App->quadTree->InsertObject(this);
	else
		App->quadTree->RemoveObject(this);
}

bool GameObject::IsStatic() const
{
	return isStatic;
}

Component * GameObject::FindComponent(const int type)
{
	Component* ret = nullptr;

	if (components.empty()) {
		return nullptr;
	}
	
	for (std::vector<Component*>::iterator it = components.begin(); it != components.end(); it++) {
		if ((*it) != nullptr) {
			if (((*it)->GetType() == type)) {
				ret = (*it);
				break;
			}
		}
	}

	return ret;
}

std::vector<Component*> GameObject::FindComponents(const int type)
{
	std::vector<Component*> ret;
	if(this != nullptr)
	if (!components.empty()) {
		for (std::vector<Component*>::iterator it = components.begin(); it != components.end(); it++) {
			if ((*it) != nullptr) {
				if(((*it)->GetType() == type))
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
			//App->res->meshes.push_back((ComponentMesh*)newComponent);
			break;
		case componentType_Material:
			if (this->FindComponents(type).empty()) {
				newComponent = new ComponentMaterial(this);
				//App->res->materials.push_back((ComponentMaterial*)newComponent);
			}
			break;
		case componentType_Transform:
			if (this->FindComponents(type).empty()) {
				newComponent = new ComponentTransform(this);
			}
			break;
		case componentType_Camera:
			if (this->FindComponents(type).empty()) {
				newComponent = new ComponentCamera(this);
			}
			break;
		case componentType_Animation:
			newComponent = new ComponentAnimation(this);
			break;
		case componentType_Bone:
			newComponent = new ComponentBone(this);
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
			case componentType_Animation:
				bufferSize = sizeof(ComponentAnimation);
				break;
			case componentType_Bone:
				bufferSize = sizeof(ComponentBone);
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

void GameObject::DestroyComponent(Component * componentPointer)
{
	if (!components.empty()) {
		for (std::vector<Component*>::iterator it = components.begin(); it != components.end(); it++) {
			if ((*it) == componentPointer && (*it) != nullptr) {
				std::vector<Component*> tmp_c;
				for (int k = 0; k < components.size(); k++) {
					if (components[k] != (*it))
						tmp_c.push_back((*it));
				}
				components = tmp_c;
				(*it)->CleanUp();
				mdelete (*it);
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
				OBB obb = tmpAABB.Transform(tmp->GetGlobalTransform());
				tmpAABB = obb.MinimalEnclosingAABB();
				aabb = tmpAABB;
			}
		}
	}
	HasAABB = true;
}

void GameObject::UpdateAABBFromMesh(ComponentMesh * mesh)
{
	if (mesh != nullptr) {

		aabb.SetNegativeInfinity();
		aabb.Enclose((float3*)mesh->vertices, mesh->num_vertices);
		aabb.TransformAsAABB(GetTransform()->GetGlobalTransform());
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

void GameObject::AddChild(GameObject * child)
{

	if (child == nullptr)
		return;

	bool isUnique = true;
	for (int i = 0; i < children.size(); i++) {
		if (children[i] == child) {
			isUnique = false;
			break;
		}
	}

	if (isUnique)
		children.push_back(child);

	OnUpdateTransform();
}

void GameObject::SetScene(ModuleScene * sce)
{
	scene = sce;
}

void GameObject::OnEditor()
{
	if(ImGui::Button("Delete GameObject")) {
		App->quadTree->RemoveObject(this);
		WantToRemoveThis();
	}
	if (ImGui::Checkbox("Static", &isStatic)) {
		SwitchStatic(isStatic);
	}
	ImGui::Separator();
	if (!components.empty()) {
		for (std::vector<Component*>::iterator it = components.begin(); it != components.end(); it++) {
			if ((*it) != nullptr) {
				ImGui::TextColored(COLOR_GREEN, (*it)->typeName);
				ImGui::Separator();
				(*it)->OnEditor();
				ImGui::Separator();
			}
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

		if (App->scene != nullptr && App->scene->GetSelected() == this) {
			flags |= ImGuiTreeNodeFlags_Selected;
			//selected = true;
		}

		if (ImGui::TreeNodeEx(name.c_str(), flags)) {

			if (ImGui::IsItemClicked(1)) {
				if (App->scene != nullptr)
				{
					App->scene->SetSelected(this);
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

void GameObject::RemoveIteration(bool toSelf) {

	if (toSelf)
		if (to_remove == true) {
			RemoveThis();
			to_remove = false;
		}

	for (int i = 0; i < children.size(); i++) {
		if (children[i] != nullptr) {
			children[i]->RemoveIteration(true);
		}
	}

}

void GameObject::RemoveThis()
{
	if (parent != nullptr)
		parent->RemoveChild(this);
	else
		CleanUp();

}

void GameObject::RemoveChild(GameObject * child)
{
	if (child == nullptr)
		return;

	std::vector<GameObject*> tmp_children;

	for (int i = 0; i < children.size(); i++) {
		if (children[i] != child)
			tmp_children.push_back(children[i]);
	}

	children = tmp_children;

	child->CleanUp();

}

void GameObject::RemoveComponent(Component * c)
{
	if (c == nullptr)
		return;

	std::vector<Component*> tmp_components;

	for (int i = 0; i < components.size(); i++) {
		if (components[i] != c)
			tmp_components.push_back(components[i]);
	}

	components = tmp_components;

	c->CleanUp();
}

void GameObject::OnUpdateTransform()
{
	float4x4 global_parent = float4x4::identity;

	if (parent != nullptr && parent->GetTransform() != nullptr)
	{
		global_parent = parent->GetTransform()->GetGlobalTransform();
	}
	if (GetTransform() != nullptr)
		GetTransform()->OnUpdateTransform(GetTransform()->GetGlobalTransform(), global_parent);

	for (uint i = 0; i < children.size(); ++i)
		children[i]->OnUpdateTransform();
}


