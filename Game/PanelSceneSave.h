#pragma once
#ifndef __PANEL_SCENESAVE_H__
#define __PANEL_SCENESAVE_H__

#include "Panel.h"

class PanelSceneSave : public Panel {
public:
	void Draw();
private:
	char scene_name[50] = "";

};

#endif // !__PANEL_SCENESAVE_H__
