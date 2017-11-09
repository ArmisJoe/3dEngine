#include "Globals.h"
#include "Application.h"
#include "ModuleInput.h"

#include "HelperFoos.h"

#define MAX_KEYS 300


ModuleInput::ModuleInput(bool start_enabled) : Module(start_enabled)
{
	name = "Input";

	keyboard = new KEY_STATE[MAX_KEYS];
	memset(keyboard, KEY_IDLE, sizeof(KEY_STATE) * MAX_KEYS);
	memset(mouse_buttons, KEY_IDLE, sizeof(KEY_STATE) * MAX_MOUSE_BUTTONS);
}

// Destructor
ModuleInput::~ModuleInput()
{
	mdelete[] keyboard;
}

// Called before render is available
bool ModuleInput::Init()
{
	LOG("Init SDL input event system");
	bool ret = true;
	SDL_Init(0);

	if(SDL_InitSubSystem(SDL_INIT_EVENTS) < 0)
	{
		LOG("SDL_EVENTS could not initialize! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}

	return ret;
}

// Called every draw update
update_status ModuleInput::PreUpdate(float dt)
{
	SDL_PumpEvents();

	const Uint8* keys = SDL_GetKeyboardState(NULL);
	
	for(int i = 0; i < MAX_KEYS; ++i)
	{
		if(keys[i] == 1)
		{
			if(keyboard[i] == KEY_IDLE)
				keyboard[i] = KEY_DOWN;
			else
				keyboard[i] = KEY_REPEAT;
		}
		else
		{
			if(keyboard[i] == KEY_REPEAT || keyboard[i] == KEY_DOWN)
				keyboard[i] = KEY_UP;
			else
				keyboard[i] = KEY_IDLE;
		}
	}

	Uint32 buttons = SDL_GetMouseState(&mouse_x, &mouse_y);

	mouse_x /= SCREEN_SIZE;
	mouse_y /= SCREEN_SIZE;
	mouse_z = 0;

	for(int i = 0; i < 5; ++i)
	{
		if(buttons & SDL_BUTTON(i))
		{
			if(mouse_buttons[i] == KEY_IDLE)
				mouse_buttons[i] = KEY_DOWN;
			else
				mouse_buttons[i] = KEY_REPEAT;
		}
		else
		{
			if(mouse_buttons[i] == KEY_REPEAT || mouse_buttons[i] == KEY_DOWN)
				mouse_buttons[i] = KEY_UP;
			else
				mouse_buttons[i] = KEY_IDLE;
		}
	}

	mouse_x_motion = mouse_y_motion = 0;

	SDL_Event e;

	mouse_wheel = false;

	while(SDL_PollEvent(&e))
	{
		ImGui_ImplSdl_ProcessEvent(&e);
		switch(e.type)
		{
			case SDL_MOUSEWHEEL:
			mouse_z = e.wheel.y;
			mouse_wheel = true;
			break;

			case SDL_MOUSEMOTION:
			mouse_x = e.motion.x / SCREEN_SIZE;
			mouse_y = e.motion.y / SCREEN_SIZE;

			mouse_x_motion = e.motion.xrel / SCREEN_SIZE;
			mouse_y_motion = e.motion.yrel / SCREEN_SIZE;
			break;

			case SDL_QUIT:
			quit = true;
			break;

			case SDL_WINDOWEVENT:
			{
				if(e.window.event == SDL_WINDOWEVENT_RESIZED)
					App->renderer3D->OnResize(e.window.data1, e.window.data2);
			}
			break;
			case SDL_DROPFILE:
			{
				dropped_filedir = e.drop.file;
				std::string filetext = GetExtension(dropped_filedir);
				int filetype = App->fs->GetTypeFromExtension(filetext.c_str());
				if (filetype == ExtensionType::et_geometry) {	// Is Geometry
					GameObject* new_geo = nullptr;
					new_geo = App->assimp->LoadGeometry(dropped_filedir);
					if (new_geo != nullptr) {
						for (int i = 0; i < new_geo->children.size(); i++) {
							App->scene->AddGameObject(App->scene->GetRoot(), new_geo->children[i]);
						}
					}
					else {
						LOG("ERROR at Loading Geometry from FILE:\n\t%s", dropped_filedir);
					}
					LOG("File Loaded:\n\t%s", dropped_filedir);
				}
				else if (filetype == ExtensionType::et_texture) {	// Is Texture
					Texture* new_tex = App->tex->LoadToDDS(dropped_filedir);
				}
				else {												// Is Default / Unknown
					LOG("Unknown file format '%s'", filetext);
				}
				
				SDL_free(dropped_filedir);
				break;
			}
		}
	}

	if(quit == true || keyboard[SDL_SCANCODE_ESCAPE] == KEY_UP)
		return UPDATE_STOP;

	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleInput::CleanUp()
{
	LOG("Quitting SDL input event subsystem");
	SDL_QuitSubSystem(SDL_INIT_EVENTS);
	return true;
}

void ModuleInput::DrawConfigPanel()
{
	if (ImGui::CollapsingHeader("Input")) {
		int x, y;
		x = GetMouseX();
		y = GetMouseY();

		string mousepos = "Mouse Position: X = " + std::to_string(x) + " Y = " + std::to_string(y);

		ImGui::Text(mousepos.c_str());

	}
}
