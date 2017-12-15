#include "ModulePicker.h"
#include "Application.h"
#include "ComponentTransform.h"

ModulePicker::ModulePicker(bool start_enabled) : Module(start_enabled)
{
	name = "Picker";
}

ModulePicker::~ModulePicker()
{
}

bool ModulePicker::Start()
{
	return true;
}

update_status ModulePicker::Update(float dt)
{
	return UPDATE_CONTINUE;
}

bool ModulePicker::CleanUp()
{
	return true;
}

GameObject * ModulePicker::RayCast(const LineSegment &segment, float& total_distance)
{
	GameObject* ret = nullptr;
	total_distance = infinite;
	IterativeRayCast(segment, total_distance, &ret);

	picked = ret;
	App->scene->SetSelected(picked);
	
	
	return ret;
}

GameObject * ModulePicker::Pick()
{
	GameObject* ret = nullptr;

	float mousex = App->input->GetMouseX(), mousey = App->input->GetMouseY();

	float dist_w = -(1.0f - (float(mousex * 2.0f)) / App->window->GetWidth());
	float dist_y = 1.0f - (float(mousey * 2.0f)) / App->window->GetHeight();

	LineSegment picker = App->camera->curr_camera->GetFrustum().UnProjectLineSegment(dist_w, dist_y);

	float distance = 0.f;

	ret = RayCast(picker, distance);
	if (ret != nullptr && distance != FLOAT_INF)
	{
		string PickedObject = "You selected: " + ret->GetName();
		LOG(PickedObject.c_str());
	}
	return ret;
}

void ModulePicker::IterativeRayCast(const LineSegment & segment, float &dist, GameObject** tocollide)
{
	vector<GameObject*> possible_collisions;
	App->quadTree->CollectIntersections(possible_collisions, segment);

	if (!App->scene->dynamic_gos_OnScreen.empty())
		for (uint i = 0; i < App->scene->dynamic_gos_OnScreen.size(); ++i)
			possible_collisions.push_back(App->scene->dynamic_gos_OnScreen[i]);


	for (uint i = 0; i < possible_collisions.size(); ++i)
	{
		GameObject* go = nullptr;
		if (possible_collisions[i] != nullptr) {

			go = possible_collisions[i];

			if (go->HasAABB == true)
			{
				vector<Component*> meshes = go->FindComponents(componentType_Mesh);
				ComponentMesh* mesh = (ComponentMesh*)meshes[0];

				LineSegment segment_local_space(segment);

				// We check the AABB first so we can avoid the whole awkward triangle process
				if (!segment_local_space.Intersects(go->aabb)) continue;

				segment_local_space.Transform(go->GetTransform()->GetGlobalTransform().Inverted());

				for (uint i = 0; i < mesh->num_indices; i += 3)
				{
					Triangle triangle;

					triangle.a.Set(&mesh->vertices[mesh->indices[i++] * 3]);
					triangle.b.Set(&mesh->vertices[mesh->indices[i++] * 3]);
					triangle.c.Set(&mesh->vertices[mesh->indices[i++] * 3]);

					float distance;
					float3 hit_point;
					if (segment_local_space.Intersects(triangle, &distance, &hit_point))
					{
						if (distance < dist)
						{
							dist = distance;
							(*tocollide) = go;
						}
					}
				}
			}
		}
	}
}