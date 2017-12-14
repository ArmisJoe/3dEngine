#include "ComponentAnimation.h"
#include "Application.h"
#include "ComponentTransform.h"

ComponentAnimation::ComponentAnimation() : Component(componentType_Animation)
{
	typeName = "Animation";
	name = "Animation";
}

ComponentAnimation::ComponentAnimation(GameObject * argparent) : Component(componentType_Animation, argparent)
{
	typeName = "Animation";
	name = "Animation";
}

ComponentAnimation::~ComponentAnimation()
{
}

void ComponentAnimation::Start()
{
	
}

void ComponentAnimation::Update(float dt)
{
	if (anim == nullptr) {
		state = as_unloaded;
		return;
	}

	if (App->game->GetGameState() == GameState::gameState_play)
		state = as_play;
	if (App->game->GetPaused() == true)
		state = as_pause;

	// Time Interactions
	switch (state) {
	case as_play:
		time += (anim->tickspersec * dt) * App->game->GetTimeMultiplier(); // Ticks/sec * sec [* xTime] = ticks
		break;
	case as_pause:
		time = time;
		break;
	case as_stop:
		time = 0;
		break;
	case as_unloaded:
		break;
	default:
		LOG("Unknown State -- going back to [Stop] state");
		state = as_stop;
		time = 0;
		break;
	}

	// Looping
	if (loop == true) {
		if (time > anim->duration)
			time -= anim->duration;
	}
	else
		if (time > anim->duration)
			time = anim->duration;

	// Bone Movement
	if (this->HasParent()) {
		for (int i = 0; i < anim->NumChannels(); i++) {
				AnimNode* b = anim->Channels[i];
				GameObject* targetGo = nullptr;
				GetGOFromNodes();
				targetGo = b->object;
				//GameObject* targetGo = CheckBoneGoMatch(this->parent, b);
				if (targetGo != nullptr) { // Now we have the Go ('BoneGo') that will move
					// Bone Debug Render
					if (drawBones == true)
						DrawBones(targetGo);
					ComponentTransform* trans = (ComponentTransform*)targetGo->FindComponent(componentType_Transform);
					if (trans != nullptr) {

						TransformKeys::VectorKey bPos = b->GetPosByTime(this->time);
						TransformKeys::QuatKey bRot = b->GetRotByTime(this->time);
						TransformKeys::VectorKey bSca = b->GetScaByTime(this->time);

						trans->SetGlobalPosition(bPos.value + targetGo->GetParent()->GetTransform()->GetPosition());
						trans->SetGlobalRotation(bRot.value * targetGo->GetParent()->GetTransform()->GetRotation());
						trans->SetGlobalScale(bSca.value);
						//trans->ChangeLocalPosition(bPos.value);
						//trans->ChangeLocalRotation(tmprot);
						//trans->ChangeLocalScale(bSca.value);
						
					}
				}
			
		}
	}
	else {
		LOG("ERROR No Parent Animation");
	}


}

void ComponentAnimation::CleanUp()
{
}

bool ComponentAnimation::isPlay() const
{
	return state == as_play;
}

bool ComponentAnimation::isPause() const
{
	return state == as_pause;
}

void ComponentAnimation::GetGOFromNodes()
{
	if (gos_filled == false)
	{
		for (int i = 0; i < anim->NumChannels(); ++i)
		{
			CollectGOs(anim->Channels[i], this->parent);
		}
		gos_filled = true;
	}
}

void ComponentAnimation::DrawBones(GameObject* boneGO)
{
	if (anim == nullptr)
		return;
	ComponentTransform* t = (ComponentTransform*)boneGO->FindComponent(componentType_Transform);
	if (t == nullptr)
		return;

	App->renderer3D->debugger->DrawAABB(boneGO->GetTransform()->GetPosition(), float3(0.1, 0.1, 0.1));
	
	for (int i = 0; i < boneGO->children.size(); i++) {
		float line_vertex[] = { t->GetPosition().x, t->GetPosition().y, t->GetPosition().z, boneGO->children[i]->GetTransform()->GetPosition().x, boneGO->children[i]->GetTransform()->GetPosition().y, boneGO->children[i]->GetTransform()->GetPosition().z };
		
		glLineWidth(1);
		glColor3f(1.0, 1.0, 0.0);
		glBegin(GL_LINES);
		glVertex3f(line_vertex[0], line_vertex[1], line_vertex[2]);
		glVertex3f(line_vertex[3], line_vertex[4], line_vertex[5]);
		glEnd();

	}
	
}

GameObject* ComponentAnimation::CheckBoneGoMatch(GameObject* go, AnimNode * b)
{
	GameObject* ret = nullptr;
	
	if (go == nullptr || b == nullptr)
		return nullptr;

	if (go->GetName() == b->name)
		return go;
	
	for (int i = 0; i < go->children.size(); i++) {
		ret = CheckBoneGoMatch(go->children[i], b);
		if (ret != nullptr)
			break;
	}

	return ret;
}

void ComponentAnimation::CollectGOs(AnimNode * node, GameObject * go)
{
	if (node == nullptr || go == nullptr)
		return;

	if (go->GetName() == node->name)
	{
		node->object = go;
		return;
	}

	for (uint i = 0; i < go->children.size(); ++i)
	{
		CollectGOs(node, go->children[i]);
	}
}

void ComponentAnimation::Enable()
{
	isActive = true;
}

void ComponentAnimation::Disable()
{
	if (isPlay())
		Stop();
	isActive = false;
}

void ComponentAnimation::OnEditor()
{
	ImGui::TextColored(COLOR_YELLOW, "Animation Name: ");
	ImGui::Text("\t%s", (anim != nullptr) ? anim->name.c_str() : "");
	ImGui::TextColored(COLOR_YELLOW, "Time: ");
	ImGui::SliderFloat("Frames", &time, 0, anim->duration, "%.0f", 1.0f);
	ImGui::Checkbox("Loop?", &loop);
}

void ComponentAnimation::Serialize(JSON_Doc * doc)
{
}

void ComponentAnimation::Load(JSON_Doc * doc)
{
}

void ComponentAnimation::Play()
{
	if(state == as_stop || state == as_pause)
		state = as_play;
}

void ComponentAnimation::Pause()
{
	if(state == as_play)
		state = as_pause;
}

void ComponentAnimation::UnPause()
{
	if(state == as_pause)
		state = as_play;
}

void ComponentAnimation::Stop()
{
	state = as_stop;
}

void ComponentAnimation::Blend(Animation * next_anim, float time)
{
}
