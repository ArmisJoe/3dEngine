#pragma once
#ifndef __PANEL_SCENE_TREE_H__
#define __PANEL_SCENE_TREE_H__

#include "Panel.h"

#include "GameObject.h"

class PanelSceneTree : public Panel {
public:
	PanelSceneTree();
	~PanelSceneTree();
public:
	void Draw();
	void SetRoot(GameObject* new_r) {
		root = new_r;
	}

private:
	GameObject* root = nullptr;

};

#endif // !__PANEL_SCENE_TREE_H__
