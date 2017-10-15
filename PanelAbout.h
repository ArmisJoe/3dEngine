#pragma once

#include "Panel.h"

#include <vector>

#ifndef __PANELABOUT_H__
#define __PANELABOUT_H__

#include "ImGui\imgui.h"

class PanelAbout : public Panel {
public:
	void Draw();

public:
	std::vector<std::string> libraries;
};

#endif // !__PANELABOUT_H__
