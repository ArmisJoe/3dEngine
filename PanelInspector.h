#pragma once
#ifndef __PANEL_INSPECTOR_H__
#define __PANEL_INSPECTOR_H__

#include "Panel.h"

class PanelInspector : public Panel {
public:
	PanelInspector();
	virtual ~PanelInspector();

	void Draw();

};

#endif // !__PANEL_INSPECTOR_H__
