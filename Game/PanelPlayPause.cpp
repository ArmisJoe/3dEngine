#include "PanelPlayPause.h"
#include "Globals.h"

void PanelPlayPause::Draw()
{
	ImGui::Begin("GameTimeManager", &active, ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoMove | ImGuiWindowFlags_NoResize);
	
	bool playbutton = (App->game->GetGameState() == gameState_play) ? true : false;
	ImGui::Checkbox("PLAY", &playbutton);
	if (App->game->GetGameState() != gameState_play && playbutton == true)
		App->game->SetGameState(gameState_play);
	if (App->game->GetGameState() == gameState_play && playbutton == false)
		App->game->SetGameState(gameState_editor);
	ImGui::SameLine();
	ImGui::Checkbox("PAUSE", &App->game->Paused);
	ImGui::SameLine();
	if (ImGui::SmallButton("Next Frame")) {
		App->game->NextFrame();
	}
	ImGui::Separator();
	float tm = App->game->GetTimeMultiplier();
	if (ImGui::SliderFloat("timeMultiplier", &tm, 0, TIMEMULTIPLIER_LIMIT, "%.0f", TIMEMULTIPLIER_STEP)) {
		App->game->SetTimeMultiplier((int)tm);
	}
	ImGui::Text("Clock:");
	char realclock[10];
	char gameclock[10];

	strcpy_s(realclock, std::to_string(App->game->GetRealTimeSc()).c_str());
	strcpy_s(gameclock, std::to_string(App->game->GetGameTimeSc()).c_str());

	int realclocki = App->game->GetRealTimeSc();
	int gameclocki = App->game->GetGameTimeSc();

	ImGui::TextColored(COLOR_RED, "Real:");
	ImGui::SameLine();
	ImGui::Text(realclock);
	ImGui::SameLine();
	ImGui::TextColored(COLOR_RED, "Game:");
	ImGui::SameLine();
	ImGui::Text(gameclock);
	ImGui::End();
}
