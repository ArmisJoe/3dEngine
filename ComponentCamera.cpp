#include "ComponentCamera.h"
#include "ModuleRenderer3D.h"
#include "Globals.h"

ComponentCamera::ComponentCamera() : Component(componentType_Camera)
{
	name = "Camera";
	frustum = new Frustum();
	frustum->nearPlaneDistance = 0.5f;
	frustum->farPlaneDistance = 500.f;
	

	frustum->verticalFov = DEGTORAD * 60.0f;
	frustum->horizontalFov = SCREEN_WIDTH / SCREEN_HEIGHT;
	aspect_ratio = SCREEN_WIDTH / SCREEN_HEIGHT;
	SetFov(60);


	//frustum.Type = FrustumType::PerspectiveFrustum;
	frustum->type = FrustumType::PerspectiveFrustum;
	frustum->pos = float3::zero;
	frustum->front = float3::unitZ;
	frustum->up = float3::unitY;

}

ComponentCamera::ComponentCamera(GameObject * argparent) : Component(componentType_Camera)
{
	name = "Camera";
	frustum = new Frustum();
	//frustum.Type = FrustumType::PerspectiveFrustum;
	frustum->type = FrustumType::PerspectiveFrustum;
	frustum->pos = float3::zero;
	frustum->front = float3::unitZ;
	frustum->up = float3::unitY;

	frustum->nearPlaneDistance = 0.5f;
	frustum->farPlaneDistance = 500.f;


	frustum->verticalFov = DEGTORAD * 60.0f;
	aspect_ratio = 1.1f;

	SetFov(60);
	frustum->WorldMatrixChanged();

	SetAspectRatio(1.1f);
}

ComponentCamera::ComponentCamera(componentType argtype, GameObject * argparent) : Component(componentType_Camera)
{
	name = "Camera";
	frustum = new Frustum();
	frustum->nearPlaneDistance = 0.5f;
	frustum->farPlaneDistance = 500.f;

	//frustum.Type = FrustumType::PerspectiveFrustum;
	frustum->type = FrustumType::PerspectiveFrustum;
	frustum->pos = float3::zero;
	frustum->front = float3::unitZ;
	frustum->up = float3::unitY;




	frustum->verticalFov = DEGTORAD * 60.0f;
	aspect_ratio = 1.1f;

	SetFov(60);
	frustum->WorldMatrixChanged();

	SetAspectRatio(1.1f);
}

ComponentCamera::~ComponentCamera()
{
	delete frustum;
}

void ComponentCamera::OnEditor()
{
}

void ComponentCamera::Start()
{
}

void ComponentCamera::Update()
{

}

void ComponentCamera::CleanUp()
{
}

void ComponentCamera::UpdateProjection()
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glLoadMatrixf((GLfloat*)frustum->ProjectionMatrix().Transposed().v);

	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

bool ComponentCamera::SetFov(float argFOV)
{
	if (argFOV < 1)
		return false; // I don't play with demons

		// fov = rad2deg(2 * atan(0.5 * width / distance));
		//frustum.horizontalFov / frustum.verticalFov = argFOV;
		// so: horitzonal = vertical * FOV(rad);
		FOV = argFOV;
		frustum->verticalFov = DEGTORAD * argFOV;
		frustum->horizontalFov = 2.f * atanf(tanf(frustum->verticalFov / 2.0f) * aspect_ratio);
		UpdateProjection();
		return true;
}

void ComponentCamera::MoveUp(const float & movement)
{
	float3 move = float3::zero;
	move += frustum->up * movement;
	frustum->Translate(move);
}

void ComponentCamera::MoveDown(const float & movement)
{
	float3 move = float3::zero;
	move -= frustum->up * movement;
	frustum->Translate(move);
}

void ComponentCamera::MoveLeft(const float & movement)
{
	float3 move = float3::zero;
	move -= frustum->WorldRight() * movement;
	frustum->Translate(move);
}

void ComponentCamera::MoveRight(const float & movement)
{
	float3 move = float3::zero;
	move += frustum->WorldRight() * movement;
	frustum->Translate(move);
}

void ComponentCamera::MoveForwards(const float & movement)
{
	float3 move = float3::zero;
	move += frustum->front * movement;
	frustum->Translate(move);
}

void ComponentCamera::MoveBackwards(const float & movement)
{
	float3 move = float3::zero;
	move -= frustum->front * movement;
	frustum->Translate(move);
}

void ComponentCamera::Rotate(const float & dx, const float & dy)
{
	if (dx != 0)
	{
		Quat rotation_x = Quat::RotateY(dx);
		frustum->front = rotation_x.Mul(frustum->front).Normalized();
		frustum->up = rotation_x.Mul(frustum->up).Normalized();
	}

	if (dy != 0)
	{
		Quat rotation_y = Quat::RotateAxisAngle(frustum->WorldRight(), dy);

		float3 new_up = rotation_y.Mul(frustum->up).Normalized();

		if (new_up.y > 0.0f)
		{
			frustum->up = new_up;
			frustum->front = rotation_y.Mul(frustum->front).Normalized();
		}
	}
}

void ComponentCamera::SetAspectRatio(const float & w, const float & h)
{
	aspect_ratio = w / h;
	SetFov(FOV);
}

void ComponentCamera::SetAspectRatio(const float & aspect_ratio)
{
	this->aspect_ratio = aspect_ratio;
	SetFov(FOV);
}

float* ComponentCamera::GetViewMatrix() const {
	float4x4 matrix = frustum->ViewMatrix();
	return (float*)matrix.Transposed().v;
}

float * ComponentCamera::GetProjectionMatrix() const
{
	return (float*)frustum->ProjectionMatrix().Transposed().v;
}
