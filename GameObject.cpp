#include "GameObject.h"

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
		for (std::list<Component*>::iterator it = components.begin(); it != components.end(); it++) {
			if ((*it) != nullptr) {
				(*it)->Update(dt);
			}
		}
	}

	return true;
}

void GameObject::CleanUp()
{
	if (!components.empty()) {
		for (std::list<Component*>::iterator it = components.begin(); it != components.end(); it++) {
			if ((*it) != nullptr) {
				(*it)->CleanUp();
				delete (*it);
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

std::list<Component*> GameObject::GetComponents(componentType type)
{
	std::list<Component*> cs;

	for (std::list<Component*>::iterator it = components.begin(); it != components.end(); it++) {
		if ((*it) != nullptr) {
			if ((*it)->type == type)
				cs.push_back((*it));
		}
	}

	return cs;
}

bool Mesh::Update(float dt)
{

	Draw();

	return true;
}

void Mesh::Draw()
{

}
