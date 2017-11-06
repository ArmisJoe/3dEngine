#pragma once
#ifndef __PANEL_INSPECTOR_H__
#define __PANEL_INSPECTOR_H__

#include "Panel.h"

#include "GameObject.h"

class ModuleScene;

class PanelInspector : public Panel {
	friend class ModuleEditorUI;
public:
	PanelInspector();
	virtual ~PanelInspector();

	void Draw();

	void SetInspected(GameObject* new_insp);

private:
	GameObject* inspected = nullptr;

};

#endif // !__PANEL_INSPECTOR_H__
