#include "Globals.h"
#include "Application.h"
#include "PhysBody3D.h"
#include "ModuleCamera3D.h"

#include "src\MathGeoLib.h"
#include "glmath.h"
#include "src\Geometry\Frustum.h"

ModuleCamera3D::ModuleCamera3D(Application* app, bool start_enabled) : Module(app, start_enabled)
{
	
	name = "Camera3D";

	CalculateViewMatrix();

	X = vec3(1.0f, 0.0f, 0.0f);
	Y = vec3(0.0f, 1.0f, 0.0f);
	Z = vec3(0.0f, 0.0f, 1.0f);

	Position = vec3(0.0f, 0.0f, 5.0f);
	Reference = vec3(0.0f, 0.0f, 0.0f);

	main_camera = new Camera();
}

ModuleCamera3D::~ModuleCamera3D()
{
	delete main_camera;
}

// -----------------------------------------------------------------
bool ModuleCamera3D::Start()
{
	LOG("Setting up the camera");
	bool ret = true;

	return ret;
}

// -----------------------------------------------------------------
bool ModuleCamera3D::CleanUp()
{
	LOG("Cleaning camera");



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
		RotateCamera();
	}
	else
	if (App->input->GetKey(SDL_SCANCODE_LALT) == KEY_REPEAT) {
		if (App->input->GetMouseButton(SDL_BUTTON_LEFT) == KEY_REPEAT)
			RotateFromReference();
	}

	// Restart camera ----------------
	if (App->input->GetKey(SDL_SCANCODE_F) == KEY_UP)
		SetCamera(vec3(0, 0, 0), 15);

	// Recalculate matrix -------------
	CalculateViewMatrix();

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

	if(!RotateAroundReference)
	{
		this->Reference = this->Position;
		this->Position += Z * 0.05f;
	}

	CalculateViewMatrix();
}

// -----------------------------------------------------------------
void ModuleCamera3D::LookAt( const vec3 &Spot)
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
	return &ViewMatrix;
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

		for (uint i = 0; i < num_vertices; i+=3)
		{
			if (dot(Position , vec3(vertices[i], vertices[i + 1], vertices[i + 2])) < dist_closest)
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

void ModuleCamera3D::RotateCamera()
{
	int dx = -App->input->GetMouseXMotion();
	int dy = -App->input->GetMouseYMotion();

	float Sensitivity = 0.25f;

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


		if (Y.y < 0.0f)
		{
			Z = vec3(0.0f, Z.y > 0.0f ? 1.0f : -1.0f, 0.0f);
			Y = cross(Z, X);
		}
	}
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
	vec3 newPos(0, 0, 0);
	float speed = 3.0f * dt;

	if (App->input->GetKey(SDL_SCANCODE_LSHIFT) == KEY_REPEAT)
		speed = 8.0f * dt;

	if (App->input->GetKey(SDL_SCANCODE_W) == KEY_REPEAT) newPos -= Z * speed;
	if (App->input->GetKey(SDL_SCANCODE_S) == KEY_REPEAT) newPos += Z * speed;


	if (App->input->GetKey(SDL_SCANCODE_A) == KEY_REPEAT) newPos -= X * speed;
	if (App->input->GetKey(SDL_SCANCODE_D) == KEY_REPEAT) newPos += X * speed;

	Position += newPos;
	Reference += newPos;
}

void ModuleCamera3D::CameraZoom(float dt)
{
	vec3 newPos(0, 0, 0);

	newPos -= Z * App->input->GetMouseWheelMotion();

	Position += newPos;
	Reference += newPos;
}


void ModuleCamera3D::SetCamera(const vec3& focus, const float& distance)
{
	Reference = focus;
	Position = Reference + Z * distance;
}

Camera::Camera()
{
	//frustum.Type = FrustumType::PerspectiveFrustum;

	frustum.pos.x = 0.0f; frustum.pos.y = 0.0f; frustum.pos.z = 5.0f;
	frustum.front.x = 0.0f; frustum.front.y = 0.0f; frustum.front.z = 1.0f;
	frustum.up.x = 0.0f; frustum.up.y = 1.0f; frustum.up.z = 0.0;

	frustum.farPlaneDistance = 200.f;
	frustum.nearPlaneDistance = 0.5f;

	frustum.horizontalFov = 0;
	frustum.verticalFov = 0;
	aspect_ratio = 1.1f;

	SetFov(60);
}

Camera::~Camera()
{
}

bool Camera::SetFov(float argFOV)
{
	if (argFOV < 1)
		return false; // I don't play with demons

	else
	{
		// fov = rad2deg(2 * atan(0.5 * width / distance));
		//frustum.horizontalFov / frustum.verticalFov = argFOV;
		// so: horitzonal = vertical * FOV(rad);
		FOV = argFOV;
		frustum.verticalFov = DEGTORAD * argFOV;
		frustum.horizontalFov = 2.f * atanf(tanf(frustum.verticalFov / 2.0f) * aspect_ratio);
		return true;
	}

	return false;
}

void Camera::MoveUp(const float & movement)
{
	float3 move = float3::zero;
	move += frustum.up * movement;
	frustum.Translate(move);
}

void Camera::MoveDown(const float & movement)
{
	float3 move = float3::zero;
	move -= frustum.up * movement;
	frustum.Translate(move);
}

void Camera::MoveLeft(const float & movement)
{
	float3 move = float3::zero;
	move -= frustum.WorldRight() * movement;
	frustum.Translate(move);
}

void Camera::MoveRight(const float & movement)
{
	float3 move = float3::zero;
	move += frustum.WorldRight() * movement;
	frustum.Translate(move);
}

void Camera::MoveForwards(const float & movement)
{
	float3 move = float3::zero;
	move += frustum.front * movement;
	frustum.Translate(move);
}

void Camera::MoveBackwards(const float & movement)
{
	float3 move = float3::zero;
	move -= frustum.front * movement;
	frustum.Translate(move);
}

