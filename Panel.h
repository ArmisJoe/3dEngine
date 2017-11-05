#pragma once

#include "ImGui\imgui.h"

class Panel {
public:
	Panel();
	virtual ~Panel();

	virtual void Start();

	virtual void Draw() = 0;

	virtual void CleanUp();

public:
	bool active = true;
	ImVec2 pos = { 0, 0 };
	ImVec2 size = { 0, 0 };

public:
	char* name;

};