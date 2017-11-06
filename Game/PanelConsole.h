#pragma once

#ifndef __PANEL_CONSOLE_H__
#define __PANEL_CONSOLE_H__


#include "Panel.h"

class PanelConsole : public Panel {
public:
	PanelConsole();
	virtual ~PanelConsole();
	
	void Draw();
	
	void Clear();
	void ConsoleLog(const char* log);

private:
	ImGuiTextBuffer text_buffer;
	bool scrollToBottom = false;

};


#endif // !__PANEL_CONSOLE_H__