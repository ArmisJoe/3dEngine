#pragma once
#ifndef __PANEL_SCENESAVE_H__
#define __PANEL_SCENESAVE_H__

#include "Panel.h"

class PanelSceneSave : public Panel {
public:
	void Draw();
	void SetSceneName(const char* str);
	void SetDirToOrigin() {
		dir_path = original_dir;
	}
	void ChangeDir(const char* new_path);
private:
	char scene_name[50] = "";
	const char* dir_path = "Assets/Scenes/";
	const char* original_dir = "Assets/Scenes/";

};

#endif // !__PANEL_SCENESAVE_H__
