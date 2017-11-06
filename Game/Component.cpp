#include "Component.h"

Component::Component(const componentType argtype) : type(argtype) {}

Component::Component(componentType argtype, GameObject * argparent) : type(argtype), parent(argparent) {}

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
