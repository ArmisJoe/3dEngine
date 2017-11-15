#include "Globals.h"
#include "Application.h"
#include "ModuleRenderer3D.h"
#include "SDL\include\SDL_opengl.h"
#include <gl/GL.h>
#include <gl/GLU.h>
#include "ComponentMesh.h"
#include "ComponentTransform.h"
//#include "glew-2.1.0\include\GL\glew.h"

#pragma comment (lib, "glu32.lib")    /* link OpenGL Utility lib     */
#pragma comment (lib, "opengl32.lib") /* link Microsoft OpenGL lib   */


ModuleRenderer3D::ModuleRenderer3D(bool start_enabled) : Module(start_enabled)
{
	name = "Renderer";
	debugger = new DebugDraw();
}

// Destructor
ModuleRenderer3D::~ModuleRenderer3D()
{}

// Called before render is available
bool ModuleRenderer3D::Init()
{
	//	LOG("Creating 3D Renderer context");
	bool ret = true;

	/*
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
	SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
	SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 1);*/

	//Create context
	context = SDL_GL_CreateContext(App->window->window);
	if (context == nullptr)
	{
		//LOG("OpenGL context could not be created! SDL_Error: %s\n", SDL_GetError());
		ret = false;
	}

	// GLEW INIT
	GLenum err = glewInit();
	if (err != GLEW_OK) {
		LOG("GLEW NOT OK");
		exit(EXIT_FAILURE);
	}
	LOG("Using Glew %s", glewGetString(GLEW_VERSION));
	LOG("Vendor: %s", glGetString(GL_VENDOR));
	LOG("Renderer: %s", glGetString(GL_RENDERER));
	LOG("OpenGL version supported %s", glGetString(GL_VERSION));
	LOG("GLSL: %s\n", glGetString(GL_SHADING_LANGUAGE_VERSION));

	if (ret == true)
	{

		//Use Vsync
		if (VSYNC && SDL_GL_SetSwapInterval(1) < 0)
			//	LOG("Warning: Unable to set VSync! SDL Error: %s\n", SDL_GetError());

			//Initialize Projection Matrix
			glMatrixMode(GL_PROJECTION);
		glLoadIdentity();

		//Check for error
		GLenum error = glGetError();
		if (error != GL_NO_ERROR)
		{
			//LOG("Error initializing OpenGL! %s\n", gluErrorString(error));
			ret = false;
		}

		//Initialize Modelview Matrix
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();

//Check for error
error = glGetError();
if (error != GL_NO_ERROR)
{
	//			LOG("Error initializing OpenGL! %s\n", gluErrorString(error));
	ret = false;
}

glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
glClearDepth(1.0f);

//Initialize clear color
glClearColor(0.f, 0.f, 0.f, 1.f);

//Check for error
error = glGetError();
if (error != GL_NO_ERROR)
{
	//			LOG("Error initializing OpenGL! %s\n", gluErrorString(error));
	ret = false;
}

GLfloat LightModelAmbient[] = { 0.0f, 0.0f, 0.0f, 1.0f };
glLightModelfv(GL_LIGHT_MODEL_AMBIENT, LightModelAmbient);

lights[0].ref = GL_LIGHT0;
lights[0].ambient.Set(0.25f, 0.25f, 0.25f, 1.0f);
lights[0].diffuse.Set(0.75f, 0.75f, 0.75f, 1.0f);
lights[0].SetPos(0.0f, 0.0f, 0.0f);
lights[0].Init();

GLfloat MaterialAmbient[] = { 1.0f, 1.0f, 1.0f, 1.0f };
glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, MaterialAmbient);

GLfloat MaterialDiffuse[] = { 1.0f, 1.0f, 1.0f, 1.0f };
glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, MaterialDiffuse);

enable_depth_test = true;

glEnable(GL_DEPTH_TEST);
glEnable(GL_CULL_FACE);
lights[0].Active(true);
glEnable(GL_LIGHTING);
glEnable(GL_COLOR_MATERIAL);
glEnable(GL_TEXTURE_2D);

glShadeModel(GL_SMOOTH);

	}

	// Projection matrix for
	OnResize(App->window->GetWidth(), App->window->GetHeight());

	return ret;
}

bool ModuleRenderer3D::Start() {
	LoadConfig();
	return true;
}

// PreUpdate: clear buffer
update_status ModuleRenderer3D::PreUpdate(float dt)
{
	glEnable(GL_LIGHTING);

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	glMatrixMode(GL_MODELVIEW);
	glLoadMatrixf(App->camera->GetViewMatrix());
	//glLoadIdentity();
	// light 0 on cam pos
	lights[0].SetPos(App->camera->curr_camera->GetPos().x, App->camera->curr_camera->GetPos().y, App->camera->curr_camera->GetPos().z);



	for (uint i = 0; i < MAX_LIGHTS; ++i)
		lights[i].Render();

	return UPDATE_CONTINUE;
}

update_status ModuleRenderer3D::Update(float dt)
{
	//for (std::list<GameObject*>::iterator go_it = App->scene->gameObjects.begin(); go_it != App->scene->gameObjects.end(); go_it++) {
	//	std::list<Component*> ms = (*go_it)->GetComponents(componentType_Mesh);
	//	for (std::list<Component*>::iterator m_it = ms.begin(); m_it != ms.end(); m_it++) {
	//		Mesh* m = (Mesh*)(*m_it);
	//		if(m == nullptr)
	//			LOG("Renderer3D: No Mesh Component Found")
	//		//HardCode for Assigment01
	//			if (!App->tex->textures.empty()) {
	//				m->tex = App->tex->textures.back();
	//			}
	//		//!_HardCode for Assigment01
	//		DrawMesh(m);
	//	}
	//}

	if (!todraw.empty())
	{
		for (std::vector<GameObject*>::iterator it = todraw.begin(); it != todraw.end(); ++it) {
			if (!(*it)->FindComponents(componentType_Mesh).empty())
			{
			std::vector<Component*> ms = (*it)->FindComponents(componentType_Mesh);
			for (std::vector<Component*>::iterator m_it = ms.begin(); m_it != ms.end(); m_it++) {
				ComponentMesh* m = (ComponentMesh*)(*m_it);
				if (m != nullptr)
				{
					ComponentMaterial* mat = nullptr;
					if (!(*it)->FindComponents(componentType_Material).empty())
						mat = (ComponentMaterial*)(*it)->FindComponents(componentType_Material).front();

					ComponentTransform* trans = nullptr;
					if (!(*it)->FindComponents(componentType_Transform).empty())
						trans = (ComponentTransform*)(*it)->FindComponents(componentType_Transform).front();
					DrawMesh(trans, m, mat);

				}
			}
			}
		}
	}

	objects_drawn = todraw.size();
	todraw.clear();

	return UPDATE_CONTINUE;
}

// PostUpdate present buffer to screen
update_status ModuleRenderer3D::PostUpdate(float dt)
{

	CustomGLAttributes(); // SET GL ATTRIBUTES TO CUSTOMIZED 

	App->physics->Draw();

	StdGLAttributes();	// RESET RENDERER OPTIONS FOR THE UI 

	App->editor->Draw();

	CustomGLAttributes();

	SDL_GL_SwapWindow(App->window->window);

	return UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleRenderer3D::CleanUp()
{
	//LOG("Destroying 3D Renderer");
	mdelete debugger;
	SDL_GL_DeleteContext(context);

	return true;
}


void ModuleRenderer3D::OnResize(const int width, const int height)
{
	glViewport(0, 0, width, height);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	ProjectionMatrix = perspective(60.0f, (float)width / (float)height, 0.125f, 512.0f);
	glLoadMatrixf(&ProjectionMatrix);

	glMatrixMode(GL_MODELVIEW);

	App->camera->main_camera->SetAspectRatio(width, height);
}

void ModuleRenderer3D::StdGLAttributes() const
{
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	GLfloat LightModelAmbient[] = { STD_AMBIENT_LIGHTING, STD_AMBIENT_LIGHTING, STD_AMBIENT_LIGHTING, 1.0f };
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, LightModelAmbient);
	GLfloat MaterialAmbient[] = { STD_MATERIAL_AMBIENT, STD_MATERIAL_AMBIENT, STD_MATERIAL_AMBIENT, 1.0f };
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, MaterialAmbient);

	glDisable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);
	glDisable(GL_LIGHTING);
	glDisable(GL_COLOR_MATERIAL);
	//glDisable(GL_TEXTURE_2D);

}

void ModuleRenderer3D::CustomGLAttributes() const
{
	if (light_model_ambient != STD_AMBIENT_LIGHTING) {
		GLfloat LightModelAmbient[] = { light_model_ambient, light_model_ambient, light_model_ambient, 1.0f };
		glLightModelfv(GL_LIGHT_MODEL_AMBIENT, LightModelAmbient);
	}
	if (material_ambient != STD_MATERIAL_AMBIENT) {
		GLfloat MaterialAmbient[] = { material_ambient, material_ambient, material_ambient, 1.0f };
		glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, MaterialAmbient);
	}

	enable_depth_test ? glEnable(GL_DEPTH_TEST) : glDisable(GL_DEPTH_TEST);
	enable_cull_face ? glEnable(GL_CULL_FACE) : glDisable(GL_CULL_FACE);
	enable_lightning ? glEnable(GL_LIGHTING) : glDisable(GL_LIGHTING);
	enable_color_material ? glEnable(GL_COLOR_MATERIAL) : glDisable(GL_COLOR_MATERIAL);
	enable_texture_2D ? glEnable(GL_TEXTURE_2D) : glDisable(GL_TEXTURE_2D);

	if (enable_wireframe)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

}

void ModuleRenderer3D::DrawConfigPanel()
{
	if (ImGui::CollapsingHeader("Render")) {
		//Check Boxes
		string log = "Objects being drawn: " + to_string(objects_drawn);

		ImGui::Text(log.c_str());

		if (ImGui::Checkbox("Depth Test", &enable_depth_test)) {
			enable_depth_test ? glEnable(GL_DEPTH_TEST) : glDisable(GL_DEPTH_TEST);
			App->parson->config->SetBool("configuration.renderer.depth_test", enable_depth_test);
			App->parson->config->Save();
		}
		ImGui::SameLine();
		if (ImGui::Checkbox("Face Culling", &enable_cull_face)) {
			enable_cull_face ? glEnable(GL_CULL_FACE) : glDisable(GL_CULL_FACE);
			App->parson->config->SetBool("configuration.renderer.cull_face", enable_cull_face);
			App->parson->config->Save();
		}
		if (ImGui::Checkbox("Lighting", &enable_lightning)) {
			enable_lightning ? glEnable(GL_LIGHTING) : glDisable(GL_LIGHTING);
			App->parson->config->SetBool("configuration.renderer.lighting", enable_lightning);
			App->parson->config->Save();
		}
		ImGui::SameLine();
		if (ImGui::Checkbox("Material Color", &enable_color_material)) {
			enable_color_material ? glEnable(GL_COLOR_MATERIAL) : glDisable(GL_COLOR_MATERIAL);
			App->parson->config->SetBool("configuration.renderer.material_color", enable_color_material);
			App->parson->config->Save();
		}
		if (ImGui::Checkbox("2D Textures", &enable_texture_2D)) {
			enable_texture_2D ? glEnable(GL_TEXTURE_2D) : glDisable(GL_TEXTURE_2D);
			App->parson->config->SetBool("configuration.renderer.2D_textures", enable_texture_2D);
			App->parson->config->Save();
		}
		if (ImGui::Checkbox("Wireframe Mode", &enable_wireframe)) {
			App->parson->config->SetBool("configuration.renderer.wireframe", enable_wireframe);
			App->parson->config->Save();
		}

		//Sliders
		if (ImGui::SliderFloat("Ambient Lighting", &light_model_ambient, 0, 1.0f)) {
			GLfloat LightModelAmbient[] = { light_model_ambient, light_model_ambient, light_model_ambient, 1.0f };
			glLightModelfv(GL_LIGHT_MODEL_AMBIENT, LightModelAmbient);
		}
		if (ImGui::SliderFloat("Material Ambient", &material_ambient, 0, 1.0f)) {
			GLfloat MaterialAmbient[] = { material_ambient, material_ambient, material_ambient, 1.0f };
			glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, MaterialAmbient);
		}

	}
}

void ModuleRenderer3D::LoadConfig() {
	enable_depth_test = App->parson->config->GetBool("configuration.renderer.depth_test");
	enable_cull_face = App->parson->config->GetBool("configuration.renderer.cull_face");
	enable_lightning = App->parson->config->GetBool("configuration.renderer.lighting");
	enable_color_material = App->parson->config->GetBool("configuration.renderer.material_color");
	enable_texture_2D = App->parson->config->GetBool("configuration.renderer.2D_textures");
	enable_wireframe = App->parson->config->GetBool("configuration.renderer.wireframe");
}

void ModuleRenderer3D::DrawMesh(ComponentTransform* trans, ComponentMesh* m, ComponentMaterial* mat)
{
	glEnableClientState(GL_VERTEX_ARRAY);
	
	if (trans != nullptr) {
		glPushMatrix();
		glMultMatrixf((GLfloat*)trans->GetLocalTransform());
	}

	glEnableClientState(GL_VERTEX_ARRAY);
	glBindBuffer(GL_ARRAY_BUFFER, m->id_vertices);

	glVertexPointer(3, GL_FLOAT, 0, NULL);

	// Applying Material
	if (mat != nullptr) {
		//Apply UV if exist
		if (m->num_UV != 0)
		{
			glEnableClientState(GL_TEXTURE_COORD_ARRAY);
			glBindBuffer(GL_ARRAY_BUFFER, m->id_UV);
			glTexCoordPointer(3, GL_FLOAT, 0, NULL);
		}

		//Diffuse Channel
		if (mat->GetTextureChannel(texType_Diffuse) != nullptr)
			glBindTexture(GL_TEXTURE_2D, (GLuint)mat->GetTextureChannel(texType_Diffuse)->id);

	}

	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m->id_indices);
	glDrawElements(GL_TRIANGLES, m->num_indices, GL_UNSIGNED_INT, NULL);


	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);

	if (trans != nullptr)
		glPopMatrix();

	/*
	glEnableClientState(GL_VERTEX_ARRAY);

	glBindBuffer(GL_ARRAY_BUFFER, m->id_vertices);
	glVertexPointer(3, GL_FLOAT, 0, NULL);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m->id_indices);

	if (mat != nullptr) {
		//Apply UV if exist
		if (m->num_UV != 0)
		{
			glEnableClientState(GL_TEXTURE_COORD_ARRAY);
			glBindBuffer(GL_ARRAY_BUFFER, m->id_UV);
			glTexCoordPointer(3, GL_FLOAT, 0, NULL);
		}

		//glEnable(GL_TEXTURE_2D);
		if (mat->GetTextureChannel(texType_Diffuse) != nullptr)
			glBindTexture(GL_TEXTURE_2D, (GLuint)mat->GetTextureChannel(texType_Diffuse)->id);

	}

	if (enable_wireframe)
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	else
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

	glDrawElements(GL_TRIANGLES, m->num_indices, GL_UNSIGNED_INT, NULL);

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	glBindTexture(GL_TEXTURE_2D, 0);

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_TEXTURE_COORD_ARRAY);


	/// Attemp 01
	//if(mesh->tex != nullptr)
	//	glBindTexture(GL_TEXTURE_2D, mesh->tex->id);
	//
	//glEnableClientState(GL_VERTEX_ARRAY);
	//glEnableClientState(GL_TEXTURE_COORD_ARRAY);
	//glBindBuffer(GL_ARRAY_BUFFER, mesh->id_vertices);
	//glTexCoordPointer(3, GL_FLOAT, 0, NULL);
	//glVertexPointer(3, GL_FLOAT, 0, NULL);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, mesh->id_indices);
	//
	//if(enable_wireframe)
	//	glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	//else
	//	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	//
	//glDrawElements(GL_TRIANGLES, mesh->num_indices, GL_UNSIGNED_INT, NULL);
	//glDisableClientState(GL_VERTEX_ARRAY);
	//glDisableClientState(GL_TEXTURE_COORD_ARRAY);
	//
	//glBindTexture(GL_TEXTURE_2D, 0);
	//glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	//glBindBuffer(GL_ARRAY_BUFFER, 0);*/
}

void ModuleRenderer3D::AddGameObjectToDraw(GameObject * obj)
{
	todraw.push_back(obj);
}
