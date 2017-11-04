#include "Globals.h"
#include "Application.h"
#include "PhysBody3D.h"
#include "ModuleCamera3D.h"

#include "src\MathGeoLib.h"
#include "glmath.h"
#include "src\Geometry\Frustum.h"

ModuleCamera3D::ModuleCamera3D(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	main_camera = new ComponentCamera();
	name = "Camera3D";
}

ModuleCamera3D::~ModuleCamera3D()
{
}

// -----------------------------------------------------------------
bool ModuleCamera3D::Start()
{
	LOG("Setting up the camera");
	bool ret = true;
	curr_camera = main_camera;
	return ret;
}

// -----------------------------------------------------------------
bool ModuleCamera3D::CleanUp()
{
	LOG("Cleaning camera");
	delete main_camera;
	curr_camera = nullptr;
	return true;
}

// -----------------------------------------------------------------
update_status ModuleCamera3D::Update(float dt)
{
	// Mouse wheel motion
	if (App->input->IsMouseWheelActive()) {
		CameraZoom(dt);
	}
	// Mouse motion ----------------

	if (App->input->GetMouseButton(SDL_BUTTON_RIGHT) == KEY_REPEAT)
	{
		MoveCamera(dt);
		RotateCamera(dt);
	}
	/*else
		if (App->input->GetKey(SDL_SCANCODE_LALT) == KEY_REPEAT) {
			if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_REPEAT)
				//RotateFromReference();
		}*/

	// Restart camera ----------------
	if (App->input->GetKey(SDL_SCANCODE_F) == KEY_UP)
		main_camera->SetPos(math::float3(0, 0, 0));
	//	SetCamera(vec3(0, 0, 0), 15);

	// Recalculate matrix -------------
	float3 corners[8];
	main_camera->GetCorners(corners);
	App->renderer3D->debugger->DrawFrustum(corners);
	//	DrawLinesList(lines, s, 5, colors);

	return UPDATE_CONTINUE;
}

// -----------------------------------------------------------------
void ModuleCamera3D::Look(const vec3 &Position, const vec3 &Reference, bool RotateAroundReference)
{
	this->Position = Position;
	this->Reference = Reference;

	Z = normalize(Position - Reference);
	X = normalize(cross(vec3(0.0f, 1.0f, 0.0f), Z));
	Y = cross(Z, X);

	if (!RotateAroundReference)
	{
		this->Reference = this->Position;
		this->Position += Z * 0.05f;
	}

	CalculateViewMatrix();
}

// -----------------------------------------------------------------
void ModuleCamera3D::LookAt(const vec3 &Spot)
{
	Reference = Spot;

	Z = normalize(Position - Reference);
	X = normalize(cross(vec3(0.0f, 1.0f, 0.0f), Z));
	Y = cross(Z, X);

	CalculateViewMatrix();
}


// -----------------------------------------------------------------
void ModuleCamera3D::Move(const vec3 &Movement)
{
	Position += Movement;
	Reference += Movement;

	CalculateViewMatrix();
}

// -----------------------------------------------------------------
float* ModuleCamera3D::GetViewMatrix()
{
	return main_camera->GetViewMatrix();
}

void ModuleCamera3D::FocusMesh(const float* vertices, const uint &num_vertices)
{
	uint	y_test = 0;
	float	highest_vertex_y = 0.0f, highest_vertex_x = 0.0f, highest_vertex_z = 0.0f;

	for (uint i = 0; i < num_vertices; ++i) {
		switch (++y_test) {
		case 1:
			if (highest_vertex_x < vertices[i])
				highest_vertex_x = vertices[i];
			break;
		case 2:
			if (highest_vertex_y < vertices[i])
				highest_vertex_y = vertices[i];
			break;
		case 3:
			if (highest_vertex_z < vertices[i])
				highest_vertex_z = vertices[i];
			y_test = 0;
			break;
		}
	}

	vec3 first_v(vertices[0], vertices[1], vertices[2]);


	Position.y = Reference.y = highest_vertex_y * PROPORTION_CAMERA;
	Position.z = Reference.z = highest_vertex_z * PROPORTION_CAMERA;
	Position.x = Reference.x = highest_vertex_x * PROPORTION_CAMERA;

	vec3 closest_vertex(first_v), furthest_vertex;

	float dist_closest = dot(Position, vec3(vertices[0], vertices[1], vertices[2]));
	float dist_furthest = 0.0f;

	for (uint i = 0; i < num_vertices; i += 3)
	{
		if (dot(Position, vec3(vertices[i], vertices[i + 1], vertices[i + 2])) < dist_closest)
		{
			closest_vertex = vec3(vertices[i], vertices[i + 1], vertices[i + 2]);
			dist_closest = dot(Position, closest_vertex);
		}
		else if (dot(Position, vec3(vertices[i], vertices[i + 1], vertices[i + 2])) > dist_furthest)
		{
			furthest_vertex = vec3(vertices[i], vertices[i + 1], vertices[i + 2]);
			dist_furthest = dot(Position, closest_vertex);
		}
	}

	vec3 midpoint = (closest_vertex + furthest_vertex) / 2;

	LookAt(midpoint);

}

// -----------------------------------------------------------------
void ModuleCamera3D::CalculateViewMatrix()
{
	ViewMatrix = mat4x4(X.x, Y.x, Z.x, 0.0f, X.y, Y.y, Z.y, 0.0f, X.z, Y.z, Z.z, 0.0f, -dot(X, Position), -dot(Y, Position), -dot(Z, Position), 1.0f);
	ViewMatrixInverse = inverse(ViewMatrix);
}

void ModuleCamera3D::RotateCamera(float dt)
{
	float Sensitivity = 0.25f;

	float dx = -App->input->GetMouseXMotion() * Sensitivity * dt;
	float dy = -App->input->GetMouseYMotion() * Sensitivity * dt;

	main_camera->Rotate(dx, dy);
}

void ModuleCamera3D::RotateFromReference()
{

	Reference = vec3(0, 0, 0);

	int dx = -App->input->GetMouseXMotion();
	int dy = -App->input->GetMouseYMotion();

	float Sensitivity = 0.25f;

	Position -= Reference;

	if (dx != 0)
	{
		float DeltaX = (float)dx * Sensitivity;
		X = rotate(X, DeltaX, vec3(0.0f, 1.0f, 0.0f));
		Y = rotate(Y, DeltaX, vec3(0.0f, 1.0f, 0.0f));
		Z = rotate(Z, DeltaX, vec3(0.0f, 1.0f, 0.0f));

	}

	if (dy != 0)
	{

		float DeltaY = (float)dy * Sensitivity;

		Y = rotate(Y, DeltaY, X);
		Z = rotate(Z, DeltaY, X);

	}
	Position = Reference + Z * length(Position);
}

void ModuleCamera3D::MoveCamera(float dt)
{
	float speed = 2.0f * dt;
	if (App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT)
		speed = 8.0f * dt;

	if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) main_camera->MoveForwards(speed);
	if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) main_camera->MoveBackwards(speed);


	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) main_camera->MoveLeft(speed);
	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) main_camera->MoveRight(speed);

	if (App->input->GetKey(SDL_SCANCODE_R) == KEY_REPEAT) main_camera->MoveUp(speed);
	if (App->input->GetKey(SDL_SCANCODE_F) == KEY_REPEAT) main_camera->MoveDown(speed);
}

void ModuleCamera3D::CameraZoom(float dt)
{
	main_camera->MoveForwards(App->input->GetMouseWheelMotion());
}


void ModuleCamera3D::SetCamera(const vec3& focus, const float& distance)
{
	Reference = focus;
	Position = Reference + Z * distance;
}