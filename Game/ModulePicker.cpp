#include "ModulePicker.h"
#include "Application.h"
#include "ComponentTransform.h"

ModulePicker::ModulePicker(Application * app, bool start_enabled) : Module(app, start_enabled)
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
	IterativeRayCast(segment, total_distance, ret);

	return ret;
}
void ModulePicker::IntersectAABB(const LineSegment & picking, std::vector<GameObject*>& DistanceList)
{
	/*GameObject* root = App->scene->GetRoot();
	GameObject* it = root;
	vector<GameObject*> visited;
	do
	{
		if (it->children.size() > 0){
			for (uint i = 0; i < it->children.size(); ++i)
			{				
				vector<GameObject*>::iterator iterator = std::find(visited.begin(), visited.end(), it->children[i]);
				if (iterator == visited.end())
				{
					it = it->children[i];
					visited.push_back(it->children[i]);
				}
			}
		continue;
		}
	} while (it != root);

	
	for (uint i = 0; i < App->scene->GetRoot()->children.size(); i++) {
		if (App->scene->GetRoot()->children[i]->children.size() > 0) {
			for (uint j = 0; j < App->scene->GetRoot()->children[i]->children.size(); j++) {
				if (picking.Intersects(App->scene->GetRoot()->children[i]->children[j]->aabb)) {
					DistanceList.push_back(App->scene->GetRoot()->children[i]->children[j]);
					LOG("AABB hit");
				}
			}
		}
		else {
			if (picking.Intersects(App->scene->GetRoot()->children[i]->aabb)) {
				DistanceList.push_back(App->scene->GetRoot()->children[i]);
				LOG("AABB hit");
			}
		}
	}*/
}



void ModulePicker::IterativeRayCast(const LineSegment & segment, float &dist, GameObject *tocollide)
{

	for (uint i = 0; i < App->scene->GetRoot()->children.size(); ++i)
	{
		GameObject* go = App->scene->GetRoot()->children[i];
		vector<Component*> meshes = go->FindComponents(componentType_Mesh);

		if (!meshes.empty())
		{
			
			ComponentMesh* mesh = (ComponentMesh*)meshes[0];

			LineSegment segment_local_space(segment);

			// We check the AABB first so we can avoid the whole awkward triangle process
			if (!segment_local_space.Intersects(go->aabb)) continue;

			segment_local_space.Transform(go->GetTransform()->GetGlobalMatrix().Inverted());		
		
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
						tocollide = go;
					}


				}
			}

			/*Triangle triangle;
			for (uint it = 0; it < mesh->num_indices;)
			{

				triangle.a.Set(mesh->vertices[mesh->indices[it++]], mesh->vertices[mesh->indices[it++]], mesh->vertices[mesh->indices[it++]]);
				triangle.b.Set(mesh->vertices[mesh->indices[it++]], mesh->vertices[mesh->indices[it++]], mesh->vertices[mesh->indices[it++]]);
				triangle.c.Set(mesh->vertices[mesh->indices[it++]], mesh->vertices[mesh->indices[it++]], mesh->vertices[mesh->indices[it++]]);

	
				if (segment_local_space.Intersects(triangle, &distance, &hit_point))
				{
					if (distance < infinite)
					{
						dist = distance;
						*tocollide = (GameObject*)go;
					}
				}
			}
			*/
		}
	}
}
