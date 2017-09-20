#include "PanelConsole.h"

PanelConsole::PanelConsole()
{
	
}

PanelConsole::~PanelConsole()
{
}

void PanelConsole::Draw()
{
	ImGui::Begin("Console", &active, ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoFocusOnAppearing);
	ImGui::TextUnformatted(text_buffer.begin());
	if (need_scroll) {
		ImGui::SetScrollHere(1.0f);
		need_scroll = false;
	}
	ImGui::End();
}

void PanelConsole::Clear()
{
	text_buffer.clear();
}

void PanelConsole::ConsoleLog(const char * log)
{
	text_buffer.append(log);
}
