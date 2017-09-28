#ifndef __ModuleWindow_H__
#define __ModuleWindow_H__

#include "Module.h"
#include "SDL/include/SDL.h"

class Application;

class ModuleWindow : public Module
{
public:

	ModuleWindow(Application* app, bool start_enabled = true);

	// Destructor
	virtual ~ModuleWindow();

	bool Init();
	bool CleanUp();

	void SetTitle(const char* title, bool save_doc = false);

	void DrawConfigPanel();

public:
	//The window we'll be rendering to
	SDL_Window* window;

	//The surface contained by the window
	SDL_Surface* screen_surface;

private:
	bool fullscreen = false;
	bool check_fsc = false;
	bool resizable = false;
	bool borderless = false;
	bool check_bdls = false;

	int w = SCREEN_WIDTH, h = SCREEN_HEIGHT;
    float brightness = 1.f;
private:

	uint GetWidth() const;
	uint GetHeight() const;
	void SetWidth(uint width);
	void SetHeigth(uint height);


	bool ChangeToFullScreen();
	bool ChangeToWindow();

	void SetBorderless();
	void SetBorder();
};

#endif // __ModuleWindow_H__