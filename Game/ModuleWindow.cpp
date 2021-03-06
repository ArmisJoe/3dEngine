#include "Globals.h"
#include "Application.h"
#include "ModuleWindow.h"
#include "ModuleCamera3D.h"

ModuleWindow::ModuleWindow( bool start_enabled) : Module(start_enabled)
{
	name = "Window";
	window = NULL;
	screen_surface = NULL;
}

// Destructor
ModuleWindow::~ModuleWindow()
{
}

// Called before render is available
bool ModuleWindow::Init()
{
	//LOG("Init SDL window & surface");
	bool ret = true;

	if (SDL_Init(SDL_INIT_VIDEO) < 0)
	{
		//LOG("SDL_VIDEO could not initialize! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}
	else
	{
		//Create window
		w = SCREEN_WIDTH * SCREEN_SIZE;
		h = SCREEN_HEIGHT * SCREEN_SIZE;
		Uint32 flags = SDL_WINDOW_OPENGL | SDL_WINDOW_SHOWN;

		//Use OpenGL 2.1
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
		SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);

		if (WIN_FULLSCREEN == true)
		{
			flags |= SDL_WINDOW_FULLSCREEN;
		}

		if (WIN_RESIZABLE == true)
		{
			flags |= SDL_WINDOW_RESIZABLE;
		}

		if (WIN_BORDERLESS == true)
		{
			flags |= SDL_WINDOW_BORDERLESS;
		}

		if (WIN_FULLSCREEN_DESKTOP == true)
		{
			flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
		}

		window = SDL_CreateWindow(TITLE, SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, w, h, flags);

		if (window == NULL)
		{
			//LOG("Window could not be created! SDL_Error: %s\n", SDL_GetError());
			ret = false;
		}
		else
		{
			//Get window surface
			screen_surface = SDL_GetWindowSurface(window);
		}
	}

	return ret;
}

bool ModuleWindow::Start() {

	LoadConfig();

	return true;
}

update_status ModuleWindow::Update(float dt)
{
	SDL_GetWindowSize(window, &w, &h);
	return UPDATE_CONTINUE;
}
// Called before quitting
bool ModuleWindow::CleanUp()
{
	//LOG("Destroying SDL window and quitting all SDL systems");

	//Destroy window
	if (window != NULL)
	{
		SDL_DestroyWindow(window);
	}

	//Quit SDL subsystems
	SDL_Quit();
	return true;
}

void ModuleWindow::SetTitle(const char* title, bool save_doc)
{
	SDL_SetWindowTitle(window, title);
	if (save_doc) {
		JSON_Doc* config = App->parson->config;
		config->SetString("application.title", title);
		config->Save();
	}
}

void ModuleWindow::DrawConfigPanel()
{
	if (ImGui::CollapsingHeader("Window")) {
		if (ImGui::Checkbox("Fullscreen", &check_fsc)) {
			resizable = false;
			App->parson->config->SetBool("configuration.window.fullscreen", fullscreen);
			App->parson->config->Save();
			if (!fullscreen)
				ChangeToFullScreen();
			else
				ChangeToWindow();
		}
		ImGui::SameLine();
		if (!check_fsc)
		{
			if (ImGui::Checkbox("Resizable", &resizable)) {
				App->parson->config->SetBool("configuration.window.resizable", resizable);
				App->parson->config->Save();
			}
			if (resizable)
			{
				if (ImGui::SliderInt("Width", (int*)&w, 0, SCREEN_WIDTH) || ImGui::SliderInt("Height", (int*)&h, 0, SCREEN_HEIGHT))
				{
					SDL_SetWindowSize(window, w, h);
					App->camera->curr_camera->SetAspectRatio(w, h);
					App->parson->config->SetNumber("configuration.window.w", w);
					App->parson->config->SetNumber("configuration.window.h", h);
					App->parson->config->Save();
				}
			}
			if (ImGui::Checkbox("Borderless", &check_bdls)) {
				if (!borderless)
					SetBorderless();
				else SetBorder();

				App->parson->config->SetBool("configuration.window.borderless", borderless);
				App->parson->config->Save();

			}
		}
		if (ImGui::SliderFloat("Brightness", &brightness, 0, 5))
		{
			if (SDL_SetWindowBrightness(window, brightness) == 0) {
				App->parson->config->SetNumber("configuration.window.brightness", brightness);
				App->parson->config->Save();
			}

		}
	}
}



bool ModuleWindow::ChangeToFullScreen()
{
	bool ret = true;

	//Create window
	Uint32 flags = SDL_WINDOW_SHOWN;

	if (fullscreen == false)
	{
		//flags |= SDL_WINDOW_FULLSCREEN;
		flags |= SDL_WINDOW_FULLSCREEN_DESKTOP;
		fullscreen = true;
		SDL_SetWindowFullscreen(window, SDL_WINDOW_FULLSCREEN_DESKTOP);
	}
	else {

	}
	if (window == NULL)
	{
		LOG("Window could not be created! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}
	else
	{
		//Get window surface
		screen_surface = SDL_GetWindowSurface(window);
	}

	return ret;
}

bool ModuleWindow::ChangeToWindow()
{
	bool ret = true;
	//Create window

	if (fullscreen == true)
	{
		fullscreen = false;
		SDL_SetWindowFullscreen(window, SDL_FALSE);
	}

	if (window == NULL)
	{
		LOG("Window could not be created! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}
	else
	{
		//Get window surface
		screen_surface = SDL_GetWindowSurface(window);
	}

	return ret;
}


void ModuleWindow::SetBorderless()
{
	borderless = true;
	SDL_SetWindowBordered(window, (SDL_bool)false);
}


void ModuleWindow::SetBorder() {
	borderless = false;
	SDL_SetWindowBordered(window, (SDL_bool)true);
}

void ModuleWindow::LoadConfig() {
	SetTitle(App->parson->config->GetString("application.title"), false);
	fullscreen = App->parson->config->GetBool("configuration.window.fullscreen");
	resizable = App->parson->config->GetBool("configuration.window.resizable");
	w = App->parson->config->GetNumber("configuration.window.w");
	h = App->parson->config->GetNumber("configuration.window.h");
	borderless = App->parson->config->GetBool("configuration.window.borderless");
	brightness = App->parson->config->GetNumber("configuration.window.brightness");
}