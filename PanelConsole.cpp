#include "PanelConsole.h"

PanelConsole::PanelConsole()
{
	name = "Console";
	size = { 500, 250 };
	pos = { 50, 300 };
}

PanelConsole::~PanelConsole()
{
}

void PanelConsole::Draw()
{
	ImGui::SetNextWindowContentSize(size);
	ImGui::SetNextWindowPos(pos);
	ImGui::Begin(name, &active, ImGuiWindowFlags_NoFocusOnAppearing);
	ImGui::TextUnformatted(text_buffer.begin());
	ImGui::End();
}

void PanelConsole::Clear()
{
	text_buffer.clear();
}

void PanelConsole::ConsoleLog(const char * log)
{
	text_buffer.append(log);
	text_buffer.append("\n");
}
