#include "ModuleGame.h"

#include "Application.h"

ModuleGame::ModuleGame(bool start_enabled) : Module(start_enabled)
{
	name = "Game";
}

ModuleGame::~ModuleGame()
{
}

bool ModuleGame::Init()
{
	return true;
}

bool ModuleGame::Start()
{
	return true;
}

update_status ModuleGame::PreUpdate(float dt)
{

	dTime = dt;

	if (App->input->GetKey(SDL_SCANCODE_ESCAPE) == KEY_DOWN && gameState == gameState_noEditor_play) {
		SetGameState(gameState_editor);
	}

	switch (gameState) {
	case gameState_editor:
		break;
	case gameState_play:
		break;
	case gameState_noEditor_play:
		break;
	default:
		LOG("Unknown Game State --- Returning to [Editor] state");
		gameState = gameState_editor;
		break;
	}

	return UPDATE_CONTINUE;
}

update_status ModuleGame::Update(float dt)
{

	if (gameState == gameState_play) {	// GameState: Play
		for (int i = 0; i < (Paused ? 0 : timeMultiplier); i++)
			UpdateGame(dt);
	}
	if (gameState == gameState_noEditor_play) {	// GameState: No Editor - Play
		for (int i = 0; i < (Paused ? 0 : timeMultiplier); i++)
			UpdateGame(dt);
	}

	return UPDATE_CONTINUE;
}

update_status ModuleGame::PostUpdate(float dt)
{
	if (tmp_gamestate != gameState) { // if Game State just Changed

		switch (gameState) {
		case gameState_editor:			// To Editor
			App->editor->ClearLog();
			break;
		case gameState_play:			// To Play
			App->editor->ClearLog();
			break;
		case gameState_noEditor_play:	// To No Editor - Play
			App->editor->ClearLog();
			break;
		default:						// To Unknown
			break;
		}

		tmp_gamestate = gameState;
	}

	return UPDATE_CONTINUE;
}

bool ModuleGame::CleanUp()
{
	return true;
}

update_status ModuleGame::UpdateGame(float dt)
{
	// ìnGame Changes Here:
	//----

	LOG("GAME UPDATE at speed x %.0f", timeMultiplier);

	return UPDATE_CONTINUE;
}

update_status ModuleGame::NextFrame()
{
	App->editor->ClearLog();
	return UpdateGame(dTime);
}

float ModuleGame::SetTimeMultiplier(float tM)
{
	if (tM > TIMEMULTIPLIER_LIMIT)
		tM = TIMEMULTIPLIER_LIMIT;
	if (tM < 0)
		tM = 0;

	timeMultiplier = tM;

	return timeMultiplier;
}

void ModuleGame::SetGameState(uint gs)
{
	if (gs < gameState_unknown)
		gameState = gs;
	else
		LOG("ERROR Setting GameState: [Unknown GameState]");
}
