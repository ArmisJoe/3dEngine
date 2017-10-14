#pragma once
#ifndef __PANEL_INSPECTOR_H__
#define __PANEL_INSPECTOR_H__

#include "Panel.h"

class ModuleScene;

class PanelInspector : public Panel {
	friend class ModuleEditorUI;
public:
	PanelInspector();
	virtual ~PanelInspector();

	void Draw();

protected:
	ModuleScene* scene;

};

#endif // !__PANEL_INSPECTOR_H__
