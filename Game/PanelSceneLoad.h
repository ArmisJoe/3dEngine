#pragma once
#ifndef __PANEL_SCENELOAD_H__
#define __PANEL_SCENELOAD_H__

#include "Panel.h"
#include "Application.h"

class PanelSceneLoad : public Panel {
public:
	void Draw();
	void SetDirToOrigin() {
		dir_path = original_dir;
	}
	void ChangeDir(const char* new_path);
private:
	const char* dir_path = "Assets/Scenes/";
	const char* original_dir = "Assets/Scenes/";
	char scene_name[50] = "";

};

#endif // !__PANEL_SCENELOAD_H__
