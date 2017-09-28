#pragma once
#include "Module.h"
#include "Globals.h"
#include "glmath.h"
#include "Light.h"

#include "glew-2.1.0\include\GL\glew.h"
#include "SDL\include\SDL_opengl.h"
#include <gl\GL.h>
#include <gl\GLU.h>

#define MAX_LIGHTS 8

#define STD_AMBIENT_LIGHTING 0.6f
#define STD_MATERIAL_AMBIENT 1.0f

class ModuleRenderer3D : public Module
{
public:
	ModuleRenderer3D(Application* app, bool start_enabled = true);
	~ModuleRenderer3D();

	bool Init();
	update_status PreUpdate(float dt);
	update_status PostUpdate(float dt);
	bool CleanUp();

	void OnResize(int width, int height);

	void StdGLAttributes();
	void CustomGLAttributes();

	void DrawConfigPanel();

public:

	Light lights[MAX_LIGHTS];
	SDL_GLContext context;
	mat3x3 NormalMatrix;
	mat4x4 ModelMatrix, ViewMatrix, ProjectionMatrix;

	bool enable_depth_test = false;
	bool enable_cull_face = false;
	bool enable_lightning = false;
	bool enable_color_material = false;
	bool enable_texture_2D = false;
	bool enable_wireframe = false;
	float light_model_ambient = 0.6f;
	float material_ambient = 1.0f;

};