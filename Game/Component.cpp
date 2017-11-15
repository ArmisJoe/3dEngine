#include "Component.h"

Component::Component(const componentType argtype) : type(argtype) {
	UID = RandomNumber();
}

Component::Component(componentType argtype, GameObject * argparent) : type(argtype), parent(argparent) {
	UID = RandomNumber();
}

bool Component::SetParent(GameObject * parent)
{
	bool ret = false;

	if (parent != nullptr) {
		if (this->parent != nullptr) {
			for (int i = 0; i < this->parent->components.size(); i++) {
				if (this->parent->components[i] == this) {
					this->parent->components.erase(this->parent->components.begin() + i);
				}
			}
		}

		this->parent = parent;

		this->parent->components.push_back(this);

		ret = true;
	}

	return ret;
}

void Component::Serialize(JSON_Doc* doc) {
	
}

void Component::RemoveThis()
{
	//if (parent != nullptr)
	//	parent->RemoveComponent(this);
	//else
	//	CleanUp();
}
