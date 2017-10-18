#include "Component.h"

bool Component::SetParent(GameObject * parent)
{
	bool ret = false;
	if (parent != nullptr) {
		this->parent = parent;
		ret = true;
	}
	return ret;
}
