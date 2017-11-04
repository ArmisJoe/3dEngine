#pragma once
#include "Module.h"
#include "Globals.h"
#include "glmath.h"
#include "Light.h"

#include "SDL\include\SDL_opengl.h"
#include <gl\GL.h>
#include <gl\GLU.h>

#include "ComponentMesh.h"
#include "ComponentMaterial.h"
#include "ModuleCamera3D.h"
#include "DebugDraw.h"

//
//#include "glew-2.1.0\include\GL\glew.h"

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
	update_status Update(float dt);
	update_status PostUpdate(float dt);
	bool CleanUp();

	void OnResize(const int width, const int height);

	void StdGLAttributes() const;
	void CustomGLAttributes() const;

	void DrawConfigPanel();

	void DrawMesh(ComponentMesh* m, ComponentMaterial* mat = nullptr);

public:

	Light lights[MAX_LIGHTS];
	SDL_GLContext context;
	DebugDraw* debugger;

	bool enable_depth_test = true;
	bool enable_cull_face = true;
	bool enable_lightning = false;
	bool enable_color_material = false;
	bool enable_texture_2D = true;
	bool enable_wireframe = false;
	float light_model_ambient = 0.6f;
	float material_ambient = 1.0f;
	mat4x4 ProjectionMatrix;
};