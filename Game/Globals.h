#pragma once

// Warning disabled ---
#pragma warning( disable : 4577 ) // Warning that exceptions are disabled
#pragma warning( disable : 4530 )

#include <windows.h>
#include <stdio.h>
#include <climits>
#include <random>


//#define LOG(format, ...) mylog(__FILE__, __LINE__, format, __VA_ARGS__);

#define CAP(n) ((n <= 0.0f) ? n=0.0f : (n >= 1.0f) ? n=1.0f : n=n)

#define DEGTORAD 0.0174532925199432957f
#define RADTODEG 57.295779513082320876f
#define HAVE_M_PI

float RandomNumber(float min = INT_MIN, float max = INT_MAX);

typedef unsigned int uint;

enum update_status
{
	UPDATE_CONTINUE = 1,
	UPDATE_STOP,
	UPDATE_ERROR
};

// Configuration -----------
#define SCREEN_WIDTH 1280
#define SCREEN_HEIGHT 1024
#define SCREEN_SIZE 1
#define WIN_FULLSCREEN false
#define WIN_RESIZABLE true
#define WIN_BORDERLESS false
#define WIN_FULLSCREEN_DESKTOP false
#define VSYNC true
#define TITLE "3D Physics Playground"

#define COLOR_RED			ImVec4(255, 0, 0, 1)
#define COLOR_GREEN			ImVec4(0, 255, 0, 1)
#define COLOR_BLUE			ImVec4(0, 0, 255, 1)
#define COLOR_CYAN			ImVec4(0, 255, 255, 1)
#define COLOR_MAGENTA		ImVec4(255, 0, 255, 1)
#define COLOR_YELLOW		ImVec4(255, 255, 0, 1)
#define COLOR_BLACK			ImVec4(0, 0, 0, 1)
#define COLOR_WHITE			ImVec4(255, 255, 255, 1)